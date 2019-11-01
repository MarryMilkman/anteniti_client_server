#include "controllers/NotificationController.hpp"
#include "controllers/notification_tools/Event.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "ScriptExecutor.hpp"
#include "Timer.hpp"
#include "Parser.hpp"
#include "TCP_IP_Worker.hpp"

#include <cerrno>

NotificationController::NotificationController() :
	_cloud_controller(CloudController::getInstance()),
	_status_controller(StatusController::getInstance()),
	_info_controller(RouterInfoController::getInstance())
{
	std::fstream 	file;

	file.open(Constant::Notification::connection_log, std::ios::out | std::ios::trunc);
	file.close();

	this->_watcher_general = 0;
	this->_watcher_general5 = 0;
	this->_watcher_sump = 0;
	this->_watcher_sump5 = 0;
	this->_watcher_guest = 0;
	this->_watcher_guest5 = 0;
	this->notify_new_connect = true;
	this->notify_common_connect = true;
	this->notify_disconnect = true;
	this->notify_general = true;
	this->notify_guest = true;
	this->notify_sump = true;

	this->_list_connected_devices = RouterInfoController::get_list_connected_devices();
	time(&this->_time_last_check);
}

NotificationController::~NotificationController() {
}

NotificationController &NotificationController::getInstance() {
    static NotificationController  _notification_controller;

    return _notification_controller;
}

// for "std::thread thread_notification"
void	NotificationController::operator()() {
	sleep (1);
	while (1) {
		// sleep(1);
		// usleep(500);
		if (this->_status_controller.getWorkMod() == eWorkMod::wm_server) {
			this->_server_behavior();
		}
		else if (this->_status_controller.getWorkMod() == eWorkMod::wm_client){
			this->_client_behavior();
		}
	}
}

void 		NotificationController::refresh_list_familiar_devices() {
	std::fstream					file;
	std::string 					line;
	std::lock_guard<std::mutex> 	g_lock(this->_list_familiar_devices_mutex);

	file.open(Constant::Notification::path_familiar_devices);
	if (!file.is_open()) {
		std::ofstream 	new_file(Constant::Notification::path_familiar_devices);

		file.open(Constant::Blocking::path_blocklist);
	}
	this->_list_familiar_mac.clear();
	while (getline(file, line)) {
		std::stringstream 	ss(line);
		std::string 		mac;

		ss >> mac;
		this->_list_familiar_mac.push_back(mac);
	}
	file.close();
		// rewrite if file familiar_devices has just been created
	file.open(Constant::Notification::path_familiar_devices);
	for (std::string mac : this->_list_familiar_mac) {
		std::map<std::string /*type*/, std::string /*value*/>	info_map;

		info_map = get_dev_info_by_mac(mac);
		file << mac << " " << info_map["nick"] << " " << info_map["ip"] << "\n";
	}
}


void		NotificationController::_server_behavior() {
	  //
	this->_tracking_self_events();
	this->_listen_sattelites_events(1);
	if (this->_is_jamming()) {
		this->_cloud_controller.notificat(Constant::Cloud::notif_jamming, "");
		return ;
	}
	this->_filter_list_events();
	this->_handle_events();
}

void		NotificationController::_client_behavior() {
	static Timer 	timer;

	if (!timer.one_time_in(1))
		return ;
	this->_tracking_self_events();
	if (this->_list_events.size())
		this->_send_events_to_master();
}



// MARK: - COMONICATE

 // for server
void 		NotificationController::_listen_sattelites_events(int timeout) {
	TCP_IP_Worker 			listen_tcp_ip;
	std::vector<RouterData>	list_routers = this->_info_controller.get_routers_info();
	int 					count = 0;
	int 					size = list_routers.size();

	// std::cerr << "start accept message\n";
	while (count < size && listen_tcp_ip.acceptMessage(Constant::Notification::notification_port, timeout) == 0) {
		std::string 	message = listen_tcp_ip.get_message();
		Event 			new_event(message);

		std::cerr << "Event from satellite\n";
		this->_list_events.push_back(new_event);
		count++;
	}
	// std::cerr << count << " notif connect\n";
}

 // for client
void 		NotificationController::_send_events_to_master() {
	TCP_IP_Worker 			tcp_ip;
	RouterData 				server = this->_info_controller.get_server_info();
	std::string 			message;
	int 					i = 0;

	if (!server.ip.size()){
		std::cerr << "ERROR: this router does not know server ip\n";
		return;
	}
	for (Event event : this->_list_events) {
		message += event.get_str();
	}
	// std::cerr << server.ip << " alllllllaaah  pomogi!\n";
	while (tcp_ip.tcp_connect(server.ip, Constant::Notification::notification_port, 1)) {
		if (i > 100)
			break ;
		usleep(200);
		i++;
	}
	if (i > 100) {
		std::cerr << "(ANSWER NOT SENDED): " << message << "\n";
		perror("reason: ");
		return ;
	}
	std::cerr << "success connect, try: " << i << "\n";
	std::cerr << "send mess: " << message << "\n";
	tcp_ip.tcp_send(message);
}




// MARK: - logics methods

 // specify new or old concret device
void 		NotificationController::_filter_list_events() {
	for (int i = 0, size = this->_list_events.size(); i < size;) {
		Event &event = this->_list_events[i];

		this->_check_fame(event);
		if (!this->notify_general && (event.iface == "wlan0" || event.iface == "wlan1-2")) {
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			continue;
		}
		if (!this->notify_guest && (event.iface == "wlan0-3" || event.iface == "wlan1-4")) {
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			continue;
		}
		if (!this->notify_sump && (event.iface == "wlan0-2" || event.iface == "wlan1-3")) {
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			continue;
		}
		i++;
	}
	this->_consistency_check_of_events();
}

void 		NotificationController::_check_fame(Event & event) {
	event.is_new = true;
	for (std::string fam_mac : this->_list_familiar_mac)
		if (fam_mac == event.mac) {
			event.is_new = false;
			break;
		}
	if (event.is_new == true) {
		std::lock_guard<std::mutex> 							g_lock(this->_list_familiar_devices_mutex);
		std::map<std::string /*type*/, std::string /*value*/>	info_map;
		std::fstream 											file_familiar_devices(Constant::Notification::path_familiar_devices);

		info_map = get_dev_info_by_mac(event.mac);
		file_familiar_devices << event.mac << " " << info_map["nick"] << " " << info_map["ip"] << "\n";
		this->_list_familiar_mac.push_back(event.mac);
	}
}

void 		NotificationController::_consistency_check_of_events() {
	std::vector<ConnectedDeviceInfo>	fresh_list_connected_devices = RouterInfoController::get_list_connected_devices();
	bool 								need_reload_list = false;

	for (int i = 0, size = this->_list_events.size(); i < size;) {
		Event	&event = this->_list_events[i];
		bool 	fresh_list_is_exist = false;
		bool 	self_list_is_exist = false;

		for (ConnectedDeviceInfo conn_dev : this->_list_connected_devices) {
			if (conn_dev._mac == event.mac) {
				self_list_is_exist = true;
				break ;
			}
		}
		if ((event.conn && self_list_is_exist) /*|| (!event.conn && !self_list_is_exist)*/) {
			std::cerr << "False event: old list\n";

			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			need_reload_list = true;
			continue;
		}
		for (ConnectedDeviceInfo conn_dev : fresh_list_connected_devices) {
			if (conn_dev._mac == event.mac) {
				fresh_list_is_exist = true;
				break ;
			}
		}
		if ((!event.conn && fresh_list_is_exist) /*|| (event.conn && !fresh_list_is_exist)*/) {
			std::cerr << "False event: fresh list\n";
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			need_reload_list = true;
			continue;
		}
		i++;
	}
	if (need_reload_list && !this->_list_connected_devices.size())
		this->_list_connected_devices = fresh_list_connected_devices;
}


 // for server and cleand mod
void 		NotificationController::_tracking_self_events() {
	this->_list_events.clear();
	this->_check_watchers();
	this->_explore_and_clean_connection_log();
}

void 		NotificationController::_explore_and_clean_connection_log() {
	std::fstream 		file(Constant::Notification::connection_log);
	std::stringstream	ss;
	std::string 		line;
	int 				event_count = 0;

	while (getline(file, line)) {
		ss << line << "\n";
		event_count++;
	}
	if (!this->_list_events.size() && !event_count)
		return;
	file.close();
	// make clean
	file.open(Constant::Notification::connection_log, std::ios::out | std::ios::trunc);
	file.close();
	while (getline(ss, line))
		this->_list_events.push_back(Event(line));
}


 // for server
 // depending on setting, notificat about event or ignore it
void 		NotificationController::_handle_events() {
	if (!this->_is_time_notify() || !this->_list_events.size())
		return ;
	for (Event event : this->_list_events) {
		std::cerr << event.get_str() << "from NotificationController::_handle_events\n";
		if (event.conn && this->notify_new_connect && event.is_new) {
			this->_cloud_controller.notificat(Constant::Cloud::notif_connect_new_d, event.nick);
			continue;
		}
		if (event.conn && this->notify_common_connect) {
			this->_cloud_controller.notificat(Constant::Cloud::notif_connect_d, event.nick);
			continue;
		}
		if (!event.conn && this->notify_disconnect)
			this->_cloud_controller.notificat(Constant::Cloud::notif_disconnect_d , event.nick);
	}

	this->_list_connected_devices = RouterInfoController::get_list_connected_devices();
	// this->_list_events.clear();
}

bool 		NotificationController::_is_time_notify() {
	time_t 	time_reload = this->_status_controller.get_time_reload();
	time_t 	now;

	time(&now);
	if (now - 4 > time_reload)
		return true;
	return false;
}

bool 		NotificationController::_is_jamming() {
	if (!this->_is_time_notify() || !this->_list_events.size())
		return false;

	int						count_dis = 0;
	int 					count_conn = 0;
	std::vector<RouterData>	list_routers;
	std::string 			script = Constant::ScriptExec::script_path + "get_nbr_conn_dev.sh";
	std::string 			answer;

	this->_info_controller.refresh();
	list_routers = this->_info_controller.get_routers_info();
	answer = ScriptExecutor::getOutput::execute(1, script.c_str());
	if (answer != "0\n" || list_routers.size())
		return false;
	for (Event event : this->_list_events) {
		if (event.conn)
			count_conn++;
		else
			count_dis++;
	}
	if (count_conn || count_dis < Constant::Notification::critical_number_dis)
		return false;
	return true;
}






  // hendl watchers metod
void 		NotificationController::_check_watchers() {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";
	int 			options = WNOHANG;
	pid_t 			term_process = waitpid(0, 0, options);

	if (term_process == 0)
		return ;
	this->_check_watchers_general(term_process);
	this->_check_watchers_sump(term_process);
	this->_check_watchers_guest(term_process);
}




void 		NotificationController::_check_watchers_general(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	if (this->_watcher_general <= 0 || this->_watcher_general == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0 -a ") + script ;//+ ">>" Constant::Notification::connection_log;
		this->_watcher_general = this->_make_watcher(script_for_exec);
		//
		// pid_t	new_pid = fork();
		// if (!new_pid) {
		// 	prctl(PR_SET_PDEATHSIG, SIGHUP);
		// 	script  = std::string("/usr/sbin/hostapd_cli -i wlan0 -a ") + script ;//+ ">>" Constant::Notification::connection_log;
		// 	std::cerr << script.c_str() << "\n";
		// 	system(script.c_str());
		// 	exit(0);
		// }
		// this->_watcher_general = new_pid;
	}
	if (this->_watcher_general5 <= 0 || this->_watcher_general5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-2 -a ") + script ;//+ ">>" Constant::Notification::connection_log;
		this->_watcher_general5 = this->_make_watcher(script_for_exec);
	}
}

void 		NotificationController::_check_watchers_sump(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	if (this->_watcher_sump <= 0 || this->_watcher_sump == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-2 -a ") + script ;//+ ">>" Constant::Notification::connection_log;
		this->_watcher_sump = this->_make_watcher(script_for_exec);
	}
	if (this->_watcher_sump5 <= 0 || this->_watcher_sump5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-3 -a ") + script ;//+ ">>" Constant::Notification::connection_log;
		this->_watcher_sump5 = this->_make_watcher(script_for_exec);
	}
}

void 		NotificationController::_check_watchers_guest(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	if (this->_watcher_guest <= 0 || this->_watcher_guest == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-3 -a ") + script ;//+ ">>" Constant::Notification::connection_log;
		this->_watcher_guest = this->_make_watcher(script_for_exec);
	}
	if (this->_watcher_guest5 <= 0 || this->_watcher_guest5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-4 -a ") + script ;//+ ">>" Constant::Notification::connection_log;
		this->_watcher_guest5 = this->_make_watcher(script_for_exec);
	}
}

pid_t 		NotificationController::_make_watcher(std::string script) {
	pid_t	new_pid = fork();
	if (!new_pid) {
		prctl(PR_SET_PDEATHSIG, SIGHUP);

		std::vector<std::string> 	script_words = Parser::custom_split(script, " ");
		char 						**env = (char **)malloc(sizeof(char *) * 2);
		int 						size = script_words.size();
		char 						**argv = (char **)malloc(sizeof(char *) * (size + 1));
		std::string 				path_log_file = "OUT=";

		path_log_file += Constant::Notification::connection_log;
		for (int i = 1; i < size; i++)
			argv[i] = (char *)script_words[i].c_str();
		argv[size] = 0;
		env[0] = (char *)path_log_file.c_str();
		env[1] = 0;
		execve(script_words[0].c_str(), argv, env);
		free(env);
		free(argv);
		exit(0);
	}
	return new_pid;
}

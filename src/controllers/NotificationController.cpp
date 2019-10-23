#include "controllers/NotificationController.hpp"
#include "controllers/notification_tools/Event.hpp"
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

	file.open(CONNECTION_LOG, std::ios::out | std::ios::trunc);
	file.close();

	this->_watcher_general = 0;
	this->_watcher_general5 = 0;
	this->_watcher_sump = 0;
	this->_watcher_sump5 = 0;
	this->_watcher_guest = 0;
	this->_watcher_guest5 = 0;
	this->notify_new_connect = false;
	this->notify_common_connect = true;
	this->notify_disconnect = true;
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

void		NotificationController::_server_behavior() {
	  //
	this->_tracking_self_events();
	this->_listen_sattelites_events(1);
	if (this->notify_new_connect)
		this->_clean_list_from_old_devices();
	this->_handle_events();
}

void		NotificationController::_client_behavior() {
	static Timer 	timer;

	if (!timer.one_time_in(1))
		return ;
	this->_tracking_self_events();
	if (this->_list_event.size())
		  //
		this->_send_events_to_master();
}



// MARK: - COMONICATE

 // for server
void 		NotificationController::_listen_sattelites_events(int timeout) {
	TCP_IP_Worker 			listen_tcp_ip;
	std::vector<RouterData>	list_routers = this->_info_controller.get_routers_info();;
	int 					count = 0;
	int 					size = list_routers.size();


	// std::cerr << "start accept message\n";
	while (count < size && listen_tcp_ip.acceptMessage(NOTIFICATION_PORT, timeout) == 0) {
		std::string 	message = listen_tcp_ip.get_message();
		Event 		new_event(message);

		this->_list_event.push_back(new_event);
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
	for (Event event : this->_list_event) {
		message += event.get_str();
	}
	// std::cerr << server.ip << " alllllllaaah  pomogi!\n";
	while (tcp_ip.tcp_connect(server.ip, NOTIFICATION_PORT, 1)) {
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
	for (Event event : this->_list_event) {
		message += event.get_str();
	}
	tcp_ip.tcp_send(message);
}




// MARK: - logics methods

 // specify new or old concret device
void 		NotificationController::_clean_list_from_old_devices() {

}

 // for server and cleand mod
void 		NotificationController::_tracking_self_events() {
	this->_list_event.clear();
	this->_check_watchers();
	this->_explore_and_clean_connection_log();
}

void 		NotificationController::_explore_and_clean_connection_log() {
	std::fstream 		file(CONNECTION_LOG);
	std::stringstream	ss;
	std::string 		line;
	int 				event_count = 0;

	while (getline(file, line)) {
		ss << line << "\n";
		event_count++;
	}
	if (!this->_list_event.size() && !event_count)
		return;
	file.close();
	// make clean
	file.open(CONNECTION_LOG, std::ios::out | std::ios::trunc);
	file.close();
	while (getline(ss, line))
		this->_list_event.push_back(Event(line));
}


 // for server
 // depending on setting, notificat about event or ignore it
void 		NotificationController::_handle_events() {
	if (!this->_is_time_notify() || !this->_list_event.size())
		return ;

	// if (this->_explore_jamming())
	// 	return ;
	for (Event event : this->_list_event) {
		if (event.conn && this->notify_new_connect)
			this->_cloud_controller.notificat(NOTIF_CONNECT_NEW_D, event.nick);
		else if (event.conn && this->notify_common_connect)
			this->_cloud_controller.notificat(NOTIF_CONNECT_D, event.nick);
		else if (!event.conn && this->notify_disconnect)
			this->_cloud_controller.notificat(NOTIF_DISCONNECT_D, event.nick);
		std::cerr << event.get_str();
	}
	// this->_list_event.clear();
}

bool 		NotificationController::_is_time_notify() {
	time_t 	time_reload = this->_status_controller.get_time_reload();
	time_t 	now;

	time(&now);
	if (now - 4 > time_reload)
		return true;
	return false;
}








  // hendl watchers metod
void 		NotificationController::_check_watchers() {
	std::string 	script = SCRIPT_PATH "testconn.sh";
	int 			options = WNOHANG;
	pid_t 			term_process = waitpid(0, 0, options);

	if (term_process == 0)
		return ;
	this->_check_watchers_general(term_process);
	this->_check_watchers_sump(term_process);
	this->_check_watchers_guest(term_process);
}




void 		NotificationController::_check_watchers_general(int term_process) {
	std::string 	script = SCRIPT_PATH "testconn.sh";

	if (this->_watcher_general <= 0 || this->_watcher_general == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0 -a ") + script ;//+ ">>" CONNECTION_LOG;
		this->_watcher_general = this->_make_watcher(script_for_exec);
		//
		// pid_t	new_pid = fork();
		// if (!new_pid) {
		// 	prctl(PR_SET_PDEATHSIG, SIGHUP);
		// 	script  = std::string("/usr/sbin/hostapd_cli -i wlan0 -a ") + script ;//+ ">>" CONNECTION_LOG;
		// 	std::cerr << script.c_str() << "\n";
		// 	system(script.c_str());
		// 	exit(0);
		// }
		// this->_watcher_general = new_pid;
	}
	if (this->_watcher_general5 <= 0 || this->_watcher_general5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-2 -a ") + script ;//+ ">>" CONNECTION_LOG;
		this->_watcher_general5 = this->_make_watcher(script_for_exec);
	}
}

void 		NotificationController::_check_watchers_sump(int term_process) {
	std::string 	script = SCRIPT_PATH "testconn.sh";

	if (this->_watcher_sump <= 0 || this->_watcher_sump == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-2 -a ") + script ;//+ ">>" CONNECTION_LOG;
		this->_watcher_sump = this->_make_watcher(script_for_exec);
	}
	if (this->_watcher_sump5 <= 0 || this->_watcher_sump5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-3 -a ") + script ;//+ ">>" CONNECTION_LOG;
		this->_watcher_sump5 = this->_make_watcher(script_for_exec);
	}
}

void 		NotificationController::_check_watchers_guest(int term_process) {
	std::string 	script = SCRIPT_PATH "testconn.sh";

	if (this->_watcher_guest <= 0 || this->_watcher_guest == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-3 -a ") + script ;//+ ">>" CONNECTION_LOG;
		this->_watcher_guest = this->_make_watcher(script_for_exec);
	}
	if (this->_watcher_guest5 <= 0 || this->_watcher_guest5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-4 -a ") + script ;//+ ">>" CONNECTION_LOG;
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

		path_log_file += CONNECTION_LOG;
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

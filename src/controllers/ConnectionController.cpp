#include "controllers/ConnectionController.hpp"
#include "EventConnect.hpp"
#include "Timer.hpp"
#include "TCP_IP_Worker.hpp"
#include "Parser.hpp"

ConnectionController::ConnectionController() :
	_status_controller(StatusController::getInstance()),
	_info_controller(RouterInfoController::getInstance()),
	_notification_controller(NotificationController::getInstance()),
	_access_controller(AccessController::getInstance())
{
}

ConnectionController::~ConnectionController() {}

ConnectionController &ConnectionController::getInstance() {
    static ConnectionController    _connection_controller;

    return _connection_controller;
}


//////////////////// ////////////////// ////////////////// /////////////////////

void 			ConnectionController::operator()() {
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


void		ConnectionController::_server_behavior() {
	this->_tracking_self_events();

	this->_listen_sattelites_events(1);
	this->_handl_connection();
	this->_notification_controller.handle_events(this->_list_events);
}

void		ConnectionController::_client_behavior() {
	static Timer 	timer;

	if (!timer.one_time_in(1))
		return ;
	this->_tracking_self_events();
	this->_handl_connection();
	if (this->_list_events.size())
		this->_send_events_to_master();
}



// MARK: - COMONICATE

 // for server
void 		ConnectionController::_listen_sattelites_events(int timeout) {
	TCP_IP_Worker 			listen_tcp_ip;
	std::vector<RouterData>	list_routers = this->_info_controller.get_routers_info();
	int 					count = 0;
	int 					size = list_routers.size();

	// std::cerr << "start accept message\n";
	while (count < size && listen_tcp_ip.acceptMessage(Constant::Notification::notification_port, timeout) == 0) {
		std::string 	message = listen_tcp_ip.get_message();
		EventConnect 			new_event(message);

		std::cerr << "EventConnect from satellite\n";
		this->_list_events.push_back(new_event);
		count++;
	}
	// std::cerr << count << " notif connect\n";
}

 // for client
void 		ConnectionController::_send_events_to_master() {
	TCP_IP_Worker 			tcp_ip;
	RouterData 				server = this->_info_controller.get_server_info();
	std::string 			message;
	int 					i = 0;

	if (!server.ip.size()){
		std::cerr << "ERROR: this router does not know server ip\n";
		return;
	}
	for (EventConnect event : this->_list_events) {
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









void 		ConnectionController::_tracking_self_events() {
	this->_list_events.clear();
	this->_check_watchers();
	this->_explore_and_clean_connection_log();
}

void 		ConnectionController::_handl_connection() {
	if (!this->_list_events.size())
		return ;
	std::lock_guard<std::mutex>						lock_access_controller(this->_access_controller.self_mutex);
	std::vector<EventConnect>								list_for_refresh;
	std::map<std::string /*mac*/, s_accessLevel> 	&map_access_level = this->_access_controller.get_map_access_level();

	for (EventConnect &event : this->_list_events) {
		if (!map_access_level.count(event.mac)) {
			list_for_refresh.push_back(event);
			event.is_new = true;
		}
	}
	if (list_for_refresh.size()) {
		this->_access_controller.refresh_tmp_map_access_level(list_for_refresh);
		this->_access_controller.apply_tmp_map_access_level();
	}
}


void 		ConnectionController::_explore_and_clean_connection_log() {
	std::fstream 		file(Constant::Files::connection_log);
	std::stringstream	ss;
	std::string 		line;
	int 				event_count = 0;

	while (getline(file, line)) {
		ss << line << "\n";
		event_count++;
	}
	if (!event_count)
		return;
	file.close();
	// make clean
	file.open(Constant::Files::connection_log, std::ios::out | std::ios::trunc);
	file.close();
	while (getline(ss, line))
		this->_list_events.push_back(EventConnect(line));
}














  // hendl watchers metod
void 		ConnectionController::_check_watchers() {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";
	int 			options = WNOHANG;
	pid_t 			term_process = waitpid(0, 0, options);

	if (term_process == 0)
		return ;
	this->_check_watchers_general(term_process);
	// this->_check_watchers_sump(term_process);
	// this->_check_watchers_guest(term_process);
}

void 		ConnectionController::_check_watchers_general(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	if (this->_watcher_general <= 0 || this->_watcher_general == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0 -a ") + script ;
		this->_watcher_general = this->_make_watcher(script_for_exec);
	}
	if (this->_watcher_general5 <= 0 || this->_watcher_general5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-1 -a ") + script ;
		this->_watcher_general5 = this->_make_watcher(script_for_exec);
	}
}

// void 		ConnectionController::_check_watchers_sump(int term_process) {
// 	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";
//
// 	if (this->_watcher_sump <= 0 || this->_watcher_sump == term_process) {
// 		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-2 -a ") + script ;
// 		this->_watcher_sump = this->_make_watcher(script_for_exec);
// 	}
// 	if (this->_watcher_sump5 <= 0 || this->_watcher_sump5 == term_process) {
// 		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-3 -a ") + script ;
// 		this->_watcher_sump5 = this->_make_watcher(script_for_exec);
// 	}
// }
//
// void 		ConnectionController::_check_watchers_guest(int term_process) {
// 	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";
//
// 	if (this->_watcher_guest <= 0 || this->_watcher_guest == term_process) {
// 		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-3 -a ") + script ;
// 		this->_watcher_guest = this->_make_watcher(script_for_exec);
// 	}
// 	if (this->_watcher_guest5 <= 0 || this->_watcher_guest5 == term_process) {
// 		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-4 -a ") + script ;
// 		this->_watcher_guest5 = this->_make_watcher(script_for_exec);
// 	}
// }

pid_t 		ConnectionController::_make_watcher(std::string script) {
	pid_t	new_pid = fork();
	if (!new_pid) {
		prctl(PR_SET_PDEATHSIG, SIGHUP);

		std::vector<std::string> 	script_words = Parser::custom_split(script, " ");
		char 						**env = (char **)malloc(sizeof(char *) * 2);
		int 						size = script_words.size();
		char 						**argv = (char **)malloc(sizeof(char *) * (size + 1));
		std::string 				path_log_file = "OUT=";

		path_log_file += Constant::Files::connection_log;
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

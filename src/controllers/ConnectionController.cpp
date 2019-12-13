#include "controllers/ConnectionController.hpp"
#include "EventConnect.hpp"
#include "Timer.hpp"
#include "TCP_IP_Worker.hpp"
#include "Parser.hpp"
#include "ScriptExecutor.hpp"
#include "AskingEntity.hpp"

ConnectionController::ConnectionController() :
	_status_controller(StatusController::getInstance()),
	_info_controller(RouterInfoController::getInstance()),
	_notification_controller(NotificationController::getInstance()),
	_access_controller(AccessController::getInstance()),
	_bc_controller(BroadcastController::getInstance())
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
		// for (int i = 0, size = this->_list_events.size(); i < size;) {
		// 	EventConnect &event = this->_list_events[i];
		//
		// 	if (!event.conn && event.iface == "e") {
		// 		std::string		scripts_for_drop_lease = "/root/drop_lease_by_mac.sh " + event.mac;
		//
		// 		ScriptExecutor::execute(1, scripts_for_drop_lease.c_str());
		// 		this->_list_events.erase(this->_list_events.begin() + i);
		// 		size = this->_list_events.size();
		// 		continue;
		// 	}
		// 	i++;
		// }
	}
}


void		ConnectionController::_server_behavior() {
	std::vector<EventConnect>	list_events_for_notify;

	this->_tracking_self_events();

	this->_listen_sattelites_events(1);
	list_events_for_notify = this->_handl_connection();
	this->_notification_controller.handle_events(list_events_for_notify);

}

void		ConnectionController::_client_behavior() {
	static Timer 				timer;
	std::vector<EventConnect>	list_events_for_notify;

	if (!timer.one_time_in(1))
		return ;
	this->_tracking_self_events();
	list_events_for_notify = this->_handl_connection();
	this->_send_events_to_master(list_events_for_notify);
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
		std::stringstream 	ss_nessage(listen_tcp_ip.get_message());
		std::string 		line;

		std::cerr << "EventConnect from satellite:\n";
		while (getline(ss_nessage, line)) {
			EventConnect 	new_event(line);

			std::cerr << line << "\n";
			new_event.is_self = false;
			this->_list_events.push_back(new_event);
		}

		count++;
	}
	// std::cerr << count << " notif connect\n";
}

 // for client
void 		ConnectionController::_send_events_to_master(std::vector<EventConnect> list_events_for_notify) {
	if (list_events_for_notify.empty())
		return ;
	TCP_IP_Worker 			tcp_ip;
	RouterData 				server = this->_info_controller.get_server_info();
	std::string 			message;
	int 					i = 0;

	if (server.ip.empty()){
		std::cerr << "ERROR: this router does not know server ip\n";
		return ;
	}
	for (EventConnect event : list_events_for_notify) {
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
	// this->_list_events.clear();
	this->_check_watchers();
	this->_explore_and_clean_connection_log();
	// for (EventConnect event : this->_list_events) {
	// 	if (event.conn)
	// 		this->_access_controller.apply_acces_level_for_mac(event.mac);
	// }
}

  // returns list events for notify
std::vector<EventConnect> 		ConnectionController::_handl_connection() {
	if (this->_list_events.empty())
		return std::vector<EventConnect>();

	eWorkMod										wm = this->_status_controller.getWorkMod();
	std::vector<EventConnect>						list_for_refresh;
	std::vector<EventConnect>						list_events_for_notify;
	std::map<std::string /*mac*/, s_accessLevel> 	map_access_level = this->_access_controller.get_map_access_level();

	for (int i = 0, size = this->_list_events.size(); i < size;) {
		EventConnect &event = this->_list_events[i];

		  // check for lease if server, in ather case for ip
		if ((wm == eWorkMod::wm_server && event.nick.empty()) || event.ip.empty()) {
			event.refresh_nick_ip();
			if (event.nbr_check_lease++ > 100) {
				event.nbr_check_lease = 0;
				this->_list_events.erase(this->_list_events.begin() + i);
				size = this->_list_events.size();
				continue;
			}
			if (event.ip.empty()) {
				AskingEntity	asking;

				if (asking.ask_everyone(Constant::Inform::ask_lease_by_mac, event.mac, 1)) {
					std::map<std::string, std::string> map_answer = asking.get_map_answer();

					for (auto item : map_answer) {
						std::stringstream	ss(item.second);
						std::string			mac_from_answer;

						ss >> mac_from_answer;
						if (mac_from_answer == event.mac) {
							ss >> event.ip;
							ss >> event.nick;
							make_ping(event.ip);
						}
					}
				}
			}
			i++;
			continue;
		}
		if (event.conn) {
			// this->_access_controller.apply_access_level_for_mac(event.mac, event.ip, event.conn);

			if (event.iface == "e") {
				// this->_info_controller.add_to_list_ethernet_mac(event.mac);
				// std::stringstream	ss_message_for_broadcast;
				//
				// ss_message_for_broadcast << Constant::Comunicate::new_connect << " " << event.mac << " " << event.iface;
				// try {
					// this->_bc_controller.send(ss_message_for_broadcast.str(), 5);
				// } catch (std::exception &e) {}
				// if (wm == wm_server)
				this->_bind_ping_to_eth(event.ip, event.mac);
			}
		}
		if (!event.conn) {
			// this->_access_controller.apply_access_level_for_mac(event.mac, event.ip, event.conn);

			if (event.iface == "e") {
				std::string		scripts_for_drop_lease = "/root/drop_lease_by_mac.sh " + event.mac;

				ScriptExecutor::execute(1, scripts_for_drop_lease.c_str());
			}
		}
		if (!map_access_level.count(event.mac)) {
			event.is_new = true;
			list_for_refresh.push_back(event);
		}
		// event.nbr_check_lease = 0;
		list_events_for_notify.push_back(this->_list_events[i]);
		this->_list_events.erase(this->_list_events.begin() + i);
		size = this->_list_events.size();
	}
	if (!list_for_refresh.empty()) {
		std::lock_guard<std::mutex>						lock_access_controller(this->_access_controller.self_mutex);

		this->_access_controller.refresh_tmp_map_access_level(list_for_refresh);
		this->_access_controller.apply_tmp_map_access_level();
	}
	return list_events_for_notify;
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
		return ;
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
	this->_check_watchers_sump(term_process);
	this->_check_watchers_guest(term_process);
	this->_check_watchers_smurt(term_process);
}

void 		ConnectionController::_check_watchers_general(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	// if (this->_watcher_general <= 0 || this->_watcher_general == term_process) {
	// 	std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-1 -a ") + script ;
	// 	this->_watcher_general = this->_make_watcher(script_for_exec);
	// }
	if (this->_watcher_general5 <= 0 || this->_watcher_general5 == term_process) {
		std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0 -a ") + script ;
		this->_watcher_general5 = this->_make_watcher(script_for_exec);
	}
}

void 		ConnectionController::_check_watchers_sump(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	// if (this->_watcher_sump <= 0 || this->_watcher_sump == term_process) {
	// 	std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-3 -a ") + script ;
	// 	this->_watcher_sump = this->_make_watcher(script_for_exec);
	// }
	// if (this->_watcher_sump5 <= 0 || this->_watcher_sump5 == term_process) {
	// 	std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-3 -a ") + script ;
	// 	this->_watcher_sump5 = this->_make_watcher(script_for_exec);
	// }
}

void 		ConnectionController::_check_watchers_guest(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	// if (this->_watcher_guest <= 0 || this->_watcher_guest == term_process) {
	// 	std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-1 -a ") + script ;
	// 	this->_watcher_guest = this->_make_watcher(script_for_exec);
	// }
	// if (this->_watcher_guest5 <= 0 || this->_watcher_guest5 == term_process) {
	// 	std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-4 -a ") + script ;
	// 	this->_watcher_guest5 = this->_make_watcher(script_for_exec);
	// }
}

void 		ConnectionController::_check_watchers_smurt(int term_process) {
	std::string 	script = Constant::ScriptExec::script_path + "testconn.sh";

	// if (this->_watcher_guest <= 0 || this->_watcher_guest == term_process) {
	// 	std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan0-2 -a ") + script ;
	// 	this->_watcher_guest = this->_make_watcher(script_for_exec);
	// }
	// if (this->_watcher_guest5 <= 0 || this->_watcher_guest5 == term_process) {
	// 	std::string	script_for_exec = std::string("/usr/sbin/hostapd_cli -i wlan1-4 -a ") + script ;
	// 	this->_watcher_guest5 = this->_make_watcher(script_for_exec);
	// }
}




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

void 		ConnectionController::_bind_ping_to_eth(std::string ip, std::string mac) {
	std::cerr << "_bind_ping_to_eth " << ip << " : " << mac << "\n";
	if (ip.empty() || mac.empty())
		return ;
	pid_t	new_pid = fork();
	if (!new_pid) {
		// eWorkMod		wm;
		std::string 	path_script = Constant::ScriptExec::script_path + "pingcheck.sh";

		std::cerr << path_script << " " << ip << "\n";
		while (1) {
			int 			count = 0;
			std::string 	answer;

			while (count < 4) {
				answer = ScriptExecutor::getOutput::execute(3, path_script.c_str(), "4", ip.c_str());
				if (answer != "0\n");
					break;
			}
			if (answer == "0\n") {
				std::ofstream	file_connection_log(Constant::Files::connection_log, std::ios::app);

				file_connection_log << "0 " << mac << " e " << time(0);
				file_connection_log.close();
				break ;
			}
			sleep(1);
		}
		exit(0);
	}
}

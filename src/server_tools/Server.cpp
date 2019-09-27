#include "Server.hpp"
#include "Timer.hpp"
#include "CustomException.hpp"


#include "TCP_IP_Worker.hpp"
#include "SSH_Worker.hpp"
#include "Parser.hpp"

Server::Server() :
	_info_controller(RouterInfoController::getInstance()),
	_bc_controller(BroadcastController::getInstance()),
	_status_controller(StatusController::getInstance()),
	_setting_controller(SettingController::getInstance()),
	_cloud_controller(CloudController::getInstance()),
	_ssh_tunnel_controller(SSHTunnelController::getInstance()),
	_blocking_controller(BlockingController::getInstance())
{
	std::cout << "Server:\n";
	this->_info_controller.refresh_satellites_list();
	// this->_get_new_key_and_notify();
	this->_init();
	this->_take_on_responsibility();
	this->_startWork();
}

Server::~Server() {
	try {
		this->_bc_controller.send(SERVER_MOD_FREE, 15);
	} catch (std::exception &e) {}
	this->_status_controller.server_availabilit = false;
}

	// first init. use by constructor only
void Server::_init() {
	this->_status_controller.server_availabilit = true;
	// setting.roolback_setting();
}


	// get new ssh key from cloud for comunicate in mesh...
	// will probably be implemented
void Server::_get_new_key_and_notify() {
	std::vector<RouterData> list_routers = this->_info_controller.get_routers_info();

	while (1) {
		bool 	is_key_reseived = true;

		try {
			this->_bc_controller.send(KEY_WAS_CHANGED, 10);
		} catch (std::exception &e) {}
		Server::_listenAnswers(list_routers, KEY_WAS_RESEIVED, LISTEN_PORT, 1);
		for (RouterData router : list_routers)
			if (!router.is_ok) {
				is_key_reseived = false;
				break ;
			}
		if (is_key_reseived)
			break ;
	}
}


	// take on responsibility:
	//	-rewrite ip config all routers in the mesh (exept self)
	//	-send broadcast, and notify mesh about self status (server mod)
	//	-refresh setting in the mesh (see metod _refresh_setting_in_mesh)
void 		Server::_take_on_responsibility() {
	std::vector<RouterData> list_routers = this->_info_controller.get_routers_info();

	RouterData				self_info = this->_info_controller.get_self_info();
	std::string 			scp_path = CONFIG_FILE_PATH;
	SSH_Worker 				ssh;
	std::ofstream 			config_file;
	std::string 			temporary_file_path = "/tmp/qwerty";

	config_file.open(temporary_file_path);
	config_file << self_info.ip;
	config_file.close();
	for (RouterData &router : list_routers) {
		ssh.login = router.login;
		ssh.ip = router.ip;
		ssh.pass = router.pass;
		std::cerr << "TUT MOJET BIT ERROR!\n";
		ssh.scp(temporary_file_path, scp_path);
	}
	system(("rm " + temporary_file_path).c_str());
	try {
		this->_bc_controller.send(SERVER_MOD_LOCK, 15);
	}
	catch (std::exception &e) {}
	// if (this->_setting_controller.is_setting_chenge())
	if (this->_setting_controller.get_version() >= 0)
		if (!this->_refresh_general_setting_in_mesh())
			std::cerr << "Cant refresh setting in the mesh... AND WE JUST IN THE BEGININ!\n";
		// while (!this->_refresh_general_setting_in_mesh()) {
		// 	std::cerr << "Cant refresh setting in the mesh... AND WE JUST IN THE BEGININ!\n";
		// 	// exit(0);
		// }
	std::cerr << "end take on responsibility\n";
}


	// general cycl of server
	//	-monitors setting file, and refresh setting in the mesh if nesessery
	// 	-one in 5 sec:
	//		a)one in 2 period (10 sec) check connection to the internet (is WAN)
	//		b)get info from mesh, and send it to Cloud
void	Server::_startWork() {
	Timer  					timer_wan;
	Timer 					timer_send_serial_number;
	std::string 			instruction;


	std::cerr << "Start work---------------hi--------------------------------\n";
	if (this->_setting_controller.is_setting_chenge())
		this->_refresh_setting_in_mesh();
	while (1) {
		if (timer_wan.one_time_in(5) && !StatusController::isWAN()) {
			std::cerr << "chalom =\\\n";
			this->_ssh_tunnel_controller.disconnect_tunnel();
			return;
		}
		// if (!this->_ssh_tunnel_controller.is_tunnel_available()) {
		// 	std::cerr << "try create ssh tunnel...\n";
		// 	this->_ssh_tunnel_controller.make_tunnel();
		// 	continue ;
		// }
		instruction = this->_ssh_tunnel_controller.get_instruction();
		if (instruction == SETTING_CHANGED) {
			if (!this->_setting_controller.is_setting_chenge())
				this->_ssh_tunnel_controller.send_message(NOTHING_TO_CHANGE);
			else {
				if (this->_refresh_setting_in_mesh())
					this->_ssh_tunnel_controller.send_message(SETTING_APPLYED);
				else {
					this->_ssh_tunnel_controller.send_message(this->_error_message);
				}
			}
		}
		if (instruction == SEND_INFO) {
			std::vector<RouterData> list_routers = this->_info_controller.get_routers_info();
			this->_ssh_tunnel_controller.send_message(MESSAGE_DELIVERED);
			this->_get_info_from_routers_and_send_to_cloud(list_routers);
		}
		if (instruction == SEND_MAC) {
			this->_ssh_tunnel_controller.send_message(this->_info_controller.get_self_info().serial_number);
		}
		if (instruction == BLOCKLIST_CHANGE) {
			if (this->_refresh_blocklist_in_mesh())
				this->_ssh_tunnel_controller.send_message(BLOCKLIST_APPLY);
			else
				this->_ssh_tunnel_controller.send_message(BLOCKLIST_NOT_APPLY);
		}
		if (timer_send_serial_number.one_time_in(60))
			this->_info_controller.refresh_satellites_list();
	}
}




// MARK : SETTING section - SETTING section - SETTING section - SETTING section

bool    	Server::_refresh_general_setting_in_mesh() {
	std::vector<RouterData>	list_routers;
	int 					version_setting = this->_setting_controller.get_version();
	int 					i = 0;

	try {
		this->_bc_controller.send(SEND_SETTING_VERSION, 10);
		list_routers = this->_info_controller.get_routers_info();
		Server::_listenAnswers(list_routers, "", LISTEN_PORT, 1);
		for (RouterData router : list_routers) {
			if (router.message == std::to_string(version_setting))
				list_routers.erase(list_routers.begin() + i);
			i++;
		}
		list_routers = this->_info_controller.get_routers_info();
		if (this->_send_and_notify_setting_chenge(PATH_SETTING, list_routers)){
			std::cerr << "setting dont send to all routers...\n";
			// this->_refresh_setting_in_mesh();
			return false;
		}
		this->_bc_controller.send(SETTING_APPLY, 10);
		list_routers = this->_info_controller.get_routers_info();
		Server::_listenAnswers(list_routers, SETTING_APPLYED, LISTEN_PORT, 10);
		for (RouterData router : list_routers)
			if (!router.is_ok) {
				std::cerr << router.ip << " fail apply: " << router.message << "\n";
				return false;
			}
		this->_bc_controller.send(SETTING_SAVE, 10);
	}
	catch (std::exception &e) {}
	std::cerr << "end of refresh general setting\n";
	return true;
}


	// refresh setting in mesh
	//	sending setting to routers, notify them, listen them answer;
	//	if answer ok -- tell them to apply setting, and then (if everything ok)
	// 	seva them.
bool 		Server::_refresh_setting_in_mesh() {
	std::vector<RouterData> list_routers = this->_info_controller.get_routers_info();

	if (this->_send_and_notify_setting_chenge(PATH_VARIABLE_SETTING, list_routers)){
		std::cerr << "setting dont send to all routers... resend setting to routers\n";
		// this->_refresh_setting_in_mesh();
		return false;
	}
	if (this->_order_applay_and_save_setting(list_routers)) {
		std::cerr << "setting dont apply in all routers... Setting was rool back\n";
		return false;
	}
	return true;
}

 	// send and notify mesh thet settings is chenged
	//	-send settings by scp to all routers
	//	-send broadcast SETTING_CHANGED
	// 	-listen answer
	//		a) everything ok - return 0;
	// 		b) some router KO - send error message to cloud and return -1;
int 		Server::_send_and_notify_setting_chenge(std::string path_to_setting, std::vector<RouterData> list_routers) {
	if (!list_routers.size())
		return 0;
	std::vector<RouterData> errors_list_routers;

	while (list_routers.size()) {
		if (this->_send_setting_to(path_to_setting, list_routers))
			return -1;
		try {
			this->_bc_controller.send(SETTING_CHANGED, 10);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << "\n";
			continue;
		}
		Server::_listenAnswers(list_routers, SETTING_DELIVERED, LISTEN_PORT, 1);
		for (RouterData router : list_routers)
			if (!router.is_ok)
				errors_list_routers.push_back(router);
		if (list_routers.size() == errors_list_routers.size()) {
			this->_form_error_message(errors_list_routers);
			return -1;
		}
		list_routers.clear();
		for (RouterData router : errors_list_routers)
			list_routers.push_back(router);
	}
	std::cerr << "Setting delivered\n";
	return 0;
}

	// send order by broadcast to apply setting, then apply self setting
	// listen answer:
	//	-if ok then send broadcast SETTING_SAVE (to save setting) and save self setting
	// 	-if not ok then send broadcast SETTING_ROOL_BACK (to roolback setting) and roolback self setting
int 		Server::_order_applay_and_save_setting(std::vector<RouterData> list_routers) {
	std::vector<RouterData> 	errors_list_routers;
	eSettingStatus				setting_status;
	std::vector<std::string>	list_unaplly_options;
	std::string 				string_with_answers_for_pars;
	bool 						is_ok = true;

	try {
		this->_bc_controller.send(SETTING_APPLY, 10);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << "\n";
		return -1;
	}
	if ((setting_status = this->_setting_controller.apply_setting()) != eSettingStatus::sApplyOK) {
		is_ok = false;
		errors_list_routers.push_back(this->_info_controller.get_self_info());
	}
	Server::_listenAnswers(list_routers, SETTING_APPLYED, LISTEN_PORT, 1);

	for (RouterData router : list_routers) {
		if (!router.is_ok) {
			string_with_answers_for_pars += router.message + "\n";
			errors_list_routers.push_back(router);
			is_ok = false;
		}
	}
	if (!is_ok) {
		if (setting_status != eSettingStatus::sApplyOK) {
			string_with_answers_for_pars += SETTING_NOT_APPLYED;
			string_with_answers_for_pars += " " + this->_setting_controller.get_str_unapply_options();
			string_with_answers_for_pars += "\n";
			std::cerr << string_with_answers_for_pars << "\n&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
			this->_info_controller.get_self_info().message = string_with_answers_for_pars;
		}
			//
		list_unaplly_options = Parser::pars_answer_apply(string_with_answers_for_pars);
		this->_form_error_message(errors_list_routers);
		try {
			std::string 	bc_message = SETTING_ROOL_BACK;

			for (std::string s : list_unaplly_options)
				bc_message += " " + s;
			this->_bc_controller.send(bc_message, 10);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << "\n";
			return -1;
		}
		this->_setting_controller.roolback_setting(list_unaplly_options);
		return -1;
	}
	else {
		std::cerr << "Setting applyed\n";
		try {
			this->_bc_controller.send(SETTING_SAVE, 10);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << "\n";
			return -1;
		}
		this->_setting_controller.save_setting();
		std::cerr << "Setting save\n";
	}
	return 0;
}

	// send the setting by scp to routers in the received list
int 	Server::_send_setting_to(std::string path_setting, std::vector<RouterData> &list_routers) {
	SSH_Worker 			ssh;

	for (RouterData router : list_routers) {
		ssh.login = router.login;
		ssh.ip = router.ip;
		ssh.pass = router.pass;
		if (ssh.scp(path_setting, PATH_VARIABLE_SETTING))
			return -1;
	}
	return 0;
}





// MARK : Blocking part

bool		Server::_refresh_blocklist_in_mesh() {
	std::vector<RouterData> 	list_routers;
	SSH_Worker 					ssh;

	this->_blocking_controller.download_list();
	list_routers = this->_info_controller.get_routers_info();
	for (RouterData router : list_routers) {
		ssh.login = router.login;
		ssh.ip = router.ip;
		ssh.pass = router.pass;
		if (ssh.scp(PATH_BLOCKLIST, PATH_BLOCKLIST))
			return false;
	}
	try {
		this->_bc_controller.send(BLOCKLIST_CHANGE, 10);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << "\n";
		return false;
	}
	return this->_blocking_controller.apply();
}










	// get info from routers and then send it to the Cloud:
	// 	scenario A:
	//		-send broadcast SEND_INFO
	//		-listen answer and story them
	//		-pars this info (make list of info)
	//		-add to list info self info
	// 		-send list info to cloud
	//	scenario B:
	//		-send broadcast SEND_INFO
	//		-all routers send info to cloud independently
	// P.S. now use scenario A
void 		Server::_get_info_from_routers_and_send_to_cloud(std::vector<RouterData> list_routers) {
	std::vector<RouterData>	error_list;
	int 					i;
	std::string 			info = "";

	i = 0;
	info += "RouterBegin" + std::to_string(++i) + std::string("\n");
	info += this->_info_controller.get_info_for_cloud();
	info += "RouterEnd" + std::to_string(i) + std::string("\n");
	try {
		this->_bc_controller.send(SEND_INFO, 10);
		Server::_listenAnswers(list_routers, "Get information...", LISTEN_PORT, 1);
	} catch (std::exception &e) {}
	for (RouterData router : list_routers) {
		info += "RouterBegin" + std::to_string(++i) + "\n";
		info += router.message;
		info += "RouterEnd" + std::to_string(i) + "\n";
		// router.is_ok = true;
		std::cerr << "Info from ip " << router.ip << ":\n";
		std::cerr << router.message << "\n- - - - - - -\n";
	}
	// parser.pars_and_add_to_answer(this->_info_controller.get_info_for_cloud());
	// std::cerr << info << "\n";
	this->_cloud_controller.post_info_to_cloud(info);
}

	// listen answer
	//		list_routers - list of routers from which need to get an answer
	//		answer_success - correct answer
	//		answer_fail - fail answer
	//		port - listen port
	// 		timeout - max time of listen answer (if timeout left,
	//				and we dont have answer - stop listening)
	//	-for routers from list set options: is_ok = false && message = ROUNER_NOT_AVAILABLE
	//	 (if the router does not send a response, its fields are in the correct position)
	//	-listen answer from list_routers
	//	-check is answer of concret router (verify by ip) is correct
	// 		a) true - parameters router.is_ok = true && router.message = ""
	// 		b) false - router.is_ok = false && router.message = answer from router
void Server::_listenAnswers(
						std::vector<RouterData> 	&list_routers,
						std::string 				answer_success,
						int                 		port,
						int 						timeout)
{
	if (!list_routers.size())
		return ;
	TCP_IP_Worker 			listener;
	int 					count = 0;
	int 					size = list_routers.size();

	for (RouterData &router : list_routers) {
		router.is_ok = false;
		router.message = ROUNER_NOT_AVAILABLE;
	}
	while (count < size && listener.acceptMessage(port, timeout) >= 0) {
		std::string message = listener.get_message();
		std::string ip = listener.get_connected_ip();

		std::cerr << ip << " connect ip, mess: " << message << "\n";
		// for (int j = 0; j < 21; j++) {
		// 	if (j > 20)
		// 		std::cerr << "...";
		// 	else
		// 		std::cerr << message[j];
		// }
		for (RouterData &router : list_routers) {
			if (router.ip == ip) {
				count++;
				if (message == answer_success) {
					router.is_ok = true;
					router.message = "";
				}
				else {
					router.is_ok = false;
					router.message = message;
				}
				break;
			}
		}
	}
}

	// sending message about error in some routers by some reason to the cloud
void 	Server::_form_error_message(std::vector<RouterData> &error_list_routers) {
	this->_error_message = "";

	for (RouterData router : error_list_routers) {
		std::stringstream 	router_error_message;

		router_error_message << "SN: " << router.serial_number << ";\nError: " << router.message << "\n";
		this->_error_message += router_error_message.str();
	};
	// std::cerr << "hello! --------------- " << this->_error_message;
}

std::mutex 		Server::_mutex;

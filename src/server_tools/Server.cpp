#include "Server.hpp"
#include "Timer.hpp"
#include "CustomException.hpp"

#include "controllers/RouterInfoController.hpp"
#include "controllers/BroadcastController.hpp"
#include "controllers/StatusController.hpp"
#include "controllers/SettingController.hpp"
#include "controllers/CloudController.hpp"

#include "TCP_IP_Worker.hpp"
#include "SSH_Worker.hpp"
#include "Parser.hpp"

Server::Server()
{
	std::cout << "Server:\n";
	this->_sendSelfInfoTo_cloud();
	// this->_get_new_key_and_notify();
	this->_init();
	this->_take_on_responsibility();
	this->_startWork();
}

Server::~Server() {
	BroadcastController::getInstance().send(SERVER_MOD_FREE, 15);
	StatusController::getInstance().server_availabilit = false;
}

	// first init. use by constructor only
void Server::_init() {
	// SettingController	&setting = SettingController::getInstance();
	SettingController::getInstance();
	StatusController::getInstance().server_availabilit = true;
	// setting.roolback_setting();
}


	// get new ssh key from cloud for comunicate in mesh...
	// will probably be implemented
void Server::_get_new_key_and_notify() {
	// CloudController			cloud_c = CloudController::getInstance();
	BroadcastController		&bc_controller = BroadcastController::getInstance();
	std::vector<RouterData> list_routers = RouterInfoController::getInstance().get_routers_info();

	while (1) {
		bool 	is_key_reseived = true;

		bc_controller.send(KEY_WAS_CHANGED, 10);
		this->_listenAnswers(list_routers, KEY_WAS_RESEIVED, "", LISTEN_PORT, 1);
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
	RouterInfoController 	&info_controller = RouterInfoController::getInstance();
	// SettingController 		&setting_controller = SettingController::getInstance();
	std::vector<RouterData> list_routers = info_controller.get_routers_info();
	RouterData				self_info = info_controller.get_self_info();
	std::string 			scp_path = CONFIG_FILE_PATH;
	SSH_Worker 				ssh;
	std::ofstream 			config_file;
	std::string 			temporary_file_path = "./qwerty";



	config_file.open(temporary_file_path);
	config_file << self_info.ip;
	config_file.close();
	for (RouterData &router : list_routers) {
		ssh.login = router.login;
		ssh.ip = router.ip;
		ssh.pass = router.pass;
		ssh.scp(temporary_file_path, scp_path);
	}
	BroadcastController::getInstance().send(SERVER_MOD_LOCK, 15);
	// if (setting_controller.is_setting_chenge())
	while (!this->_refresh_general_setting_in_mesh()) {
		std::cerr << "Cant refresh setting in the mesh... AND WE JUST IN THE BEGININ!\n";
		// exit(0);
	}
}


	// general cycl of server
	//	-monitors setting file, and refresh setting in the mesh if nesessery
	// 	-one in 5 sec:
	//		a)one in 2 period (10 sec) check connection to the internet (is WAN)
	//		b)get info from mesh, and send it to Cloud
void	Server::_startWork() {
	SettingController 		&setting_controller = SettingController::getInstance();
	Timer  					timer;
	int 					iteration = 0;

	std::cerr << "Start work-------------------------------------------------\n";

	if (!setting_controller.is_setting_chenge() && !StatusController::isWAN())
		return ;
	while (1) {
		if (setting_controller.is_setting_chenge()) {
			this->_refresh_setting_in_mesh();
		}
		if (timer.one_time_in(5)) {
			std::vector<RouterData> list_routers = RouterInfoController::getInstance().get_routers_info();

			if (!iteration % 2 && !StatusController::isWAN()) {
				// go to destructor
				return;
			}
			if (!(iteration % 20))
				this->_sendSelfInfoTo_cloud();
			iteration++;
			this->_get_info_from_routers_and_send_to_cloud(list_routers);
		}
		// if (iteration > 1 )
	}
}



// MARK : SETTING section - SETTING section - SETTING section - SETTING section

bool    	Server::_refresh_general_setting_in_mesh() {
	// SettingController		&sett_cont = SettingController::getInstance();
	BroadcastController 	&bc_controller = BroadcastController::getInstance();
	std::vector<RouterData>	list_routers = RouterInfoController::getInstance().get_routers_info();
	// std::string 			version_setting = sett_cont.get_version();
	std::string 			version_setting = "sett_cont.get_version()";
	int 					i = 0;

	bc_controller.send(SEND_SETTING_VERSION, 10);
	this->_listenAnswers(list_routers, "", "", LISTEN_PORT, 1);
	for (RouterData router : list_routers) {
		if (router.message == version_setting)
			list_routers.erase(list_routers.begin() + i);
		i++;
	}
	this->_send_setting_to(PATH_SETTING, list_routers);
	bc_controller.send(SETTING_DELIVERED, 10);
	this->_listenAnswers(list_routers, "", "", LISTEN_PORT, 1);
	bc_controller.send(SETTING_APPLY, 10);
	this->_listenAnswers(list_routers, "", "", LISTEN_PORT, 1);
	bc_controller.send(SETTING_SAVE, 10);
	this->_listenAnswers(list_routers, "", "", LISTEN_PORT, 1);
	return true;
}


	// refresh setting in mesh
	//	sending setting to routers, notify them, listen them answer;
	//	if answer ok -- tell them to apply setting, and then (if everything ok)
	// 	seva them.
bool 		Server::_refresh_setting_in_mesh() {
	std::vector<RouterData> list_routers = RouterInfoController::getInstance().get_routers_info();

	if (this->_send_and_notify_setting_chenge(list_routers)){
		std::cerr << "setting dont send to all routers... resend setting to routers\n";
		// this->_refresh_setting_in_mesh();
		return false;
	}
	if (this->_order_applay_and_save_setting(list_routers)) {
		std::cerr << "setting dont apply in all routers... Setting was rool back\n";
		return true;
	}
	return true;
}

 	// send and notify mesh thet settings is chenged
	//	-send settings by scp to all routers
	//	-send broadcast SETTING_CHENGED
	// 	-listen answer
	//		a) everything ok - return 0;
	// 		b) some router KO - send error message to cloud and return -1;
int 		Server::_send_and_notify_setting_chenge(std::vector<RouterData> list_routers) {
	if (!list_routers.size())
		return 0;
	std::vector<RouterData> errors_list_routers;
	BroadcastController		&bc_controller = BroadcastController::getInstance();

	while (list_routers.size()) {
		if (this->_send_setting_to(PATH_VARIABLE_SETTING, list_routers))
			return -1;
		try {
			bc_controller.send(SETTING_CHENGED, 10);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << "\n";
			continue;
		}
		this->_listenAnswers(list_routers, SETTING_DELIVERED, SETTING_NOT_DELIVERED, LISTEN_PORT, 1);
		for (RouterData router : list_routers)
			if (!router.is_ok)
				errors_list_routers.push_back(router);
		if (list_routers.size() == errors_list_routers.size()) {
			this->_sendErrorTo_cloud(errors_list_routers);
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
	BroadcastController			&bc_controller = BroadcastController::getInstance();
	SettingController 			&setting_controller = SettingController::getInstance();
	eSettingStatus				setting_status;
	std::vector<std::string>	list_unaplly_setting;
	std::string 				string_with_answers_for_pars;
	bool 						is_ok = true;

	try {
		bc_controller.send(SETTING_APPLY, 10);
	}
	catch (std::exception &e) {
		std::cerr << e.what() << "\n";
		return -1;
	}
	setting_status = setting_controller.apply_setting();
	this->_listenAnswers(list_routers, SETTING_APPLYED, SETTING_NOT_APPLYED, LISTEN_PORT, 1);

	for (RouterData router : list_routers) {
		string_with_answers_for_pars += router.message + "\n";
		if (!router.is_ok) {
			errors_list_routers.push_back(router);
			is_ok = false;
		}
	}
	if (!is_ok) {
		if (setting_status != eSettingStatus::sApplyOK) {
			string_with_answers_for_pars += SETTING_NOT_APPLYED;
			for (Setting self_setting : setting_controller.get_list_unapply_setting())
				string_with_answers_for_pars += " " + self_setting.option;
			string_with_answers_for_pars += "\n";
		}
			//
		list_unaplly_setting = Parser::pars_answer_apply(string_with_answers_for_pars);
		this->_sendErrorTo_cloud(errors_list_routers);
		try {
			std::string 	bc_message = SETTING_ROOL_BACK;

			for (std::string s : list_unaplly_setting)
				bc_message += " " + s;
			bc_controller.send(bc_message, 10);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << "\n";
			return -1;
		}
		setting_controller.roolback_setting(list_unaplly_setting);
		return -1;
	}
	else {
		std::cerr << "Setting applyed\n";
		try {
			bc_controller.send(SETTING_SAVE, 10);
		}
		catch (std::exception &e) {
			std::cerr << e.what() << "\n";
			return -1;
		}
		setting_controller.save_setting();
		std::cerr << "Setting save\n";
	}
	return 0;
}

	// send the setting by scp to routers in the received list
int 	Server::_send_setting_to(std::string path_setting, std::vector<RouterData> &list_routers) {
	RouterInfoController &info_controller = RouterInfoController::getInstance();
	RouterData 			server = info_controller.get_server_info();
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

// SETTING section -- END -- SETTING section -- END -- SETTING section -- END -- SETTING section





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
	BroadcastController		&bc_controller = BroadcastController::getInstance();
	RouterInfoController 	&info_controller = RouterInfoController::getInstance();
	std::vector<RouterData>	error_list;
	int 					i;
	std::string 			info;

	i = 0;
	info += "DeviceBegin" + ++i;
	info += info_controller.get_info_for_cloud();
	info += "DeviceEnd" + i;
	bc_controller.send(SEND_INFO, 10);
	this->_listenAnswers(list_routers, "Get information...", "", LISTEN_PORT, 1);
	for (RouterData router : list_routers) {
		info += "DeviceBegin" + std::to_string(++i) + "\n";
		info += router.message;
		info += "DeviceEnd" + std::to_string(i) + "\n";
		// router.is_ok = true;
		std::cerr << "Info from ip " << router.ip << ":\n";
		std::cerr << router.message << "\n- - - - - - -\n";
	}
	// parser.pars_and_add_to_answer(info_controller.get_info_for_cloud());

	CloudController 	&cloud = CloudController::getInstance();

	cloud.post_info_to_cloud(info);
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
						std::string					answer_fail,
						int                 		port,
						int 						timeout)
{
	if (!list_routers.size())
		return ;
	TCP_IP_Worker 			listener;
	int 					count = 0;
	int 					size = list_routers.size();

	if (answer_fail == "")
		count = 0;
	for (RouterData &router : list_routers) {
		router.is_ok = false;
		router.message = ROUNER_NOT_AVAILABLE;
	}
	while (count < size && listener.acceptMessage(port, timeout) >= 0) {
		std::string message = listener.get_message();
		std::string ip = listener.get_connected_ip();

		std::cerr << ip << " connect ip\n";
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

	// send self info to cloud...
int  	Server::_sendSelfInfoTo_cloud() {
	return 0;
}

	// sending message about error in some routers by some reason to the cloud
void 	Server::_sendErrorTo_cloud(std::vector<RouterData> &error_list_routers) {
	for (RouterData router : error_list_routers);
}

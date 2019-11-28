#include "Client.hpp"
#include "CustomException.hpp"
#include "ScriptExecutor.hpp"

#include "TCP_IP_Worker.hpp"



#include "Timer.hpp"
#include "Parser.hpp"

Client::~Client(){}

Client::Client() :
   _info_controller(RouterInfoController::getInstance()),
   _bc_controller(BroadcastController::getInstance()),
   _status_controller(StatusController::getInstance()),
   _setting_controller(SettingController::getInstance()),
   _access_controller(AccessController::getInstance())
{
	std::cerr << "Client:\n";
	// this->_get_key();
	this->_startWork();
}



void 	Client::_startWork() {

	while (1) {
		this->_listenBroadcast(10);
		if (!this->_status_controller.server_availabilit) {
			if (/*this->_setting_controller.is_setting_chenge() &&*/ StatusController::isWAN()) {
				throw CustomException(eExceptType::e_need_server_work_mod);
			}
		}
	}
}

bool 	Client::_listenBroadcast(int timeout) {
	std::stringstream 		ss;
	std::string				order;

	this->_status_controller.server_availabilit = false;
	if (this->_bc_controller.receive(timeout))
		return false;
	ss << this->_bc_controller.get_message();
	ss >> order;
	std::cerr << "Message geting: *" << order << "*\n";
	this->_status_controller.server_availabilit = true;
	if (order == Constant::Comunicate::send_info)
		this->_sendSelfInfo();
	else if (order == Constant::Comunicate::new_connect) {
		std::string mac;
		std::string iface;

		ss >> mac;
		ss >> iface;
		this->_access_controller.apply_access_level_for_mac(mac, iface);
	}
	else if (order == Constant::Comunicate::server_mod_free || order == Constant::Comunicate::wan_changed)
		this->_status_controller.server_availabilit = false;
	// else if (order == KEY_WAS_CHANGED)
	// 	this->_get_key();
	// else if (order == SEND_SETTING_VERSION) {
	// 	// int version = this->_setting_controller.get_version();
	// 	this->_sendAnswer(std::to_string(version), Constant::TCP_IP::listen_port);
	// }

	else if (order == Constant::Comunicate::block_list_changed) {
		std::string 	path_to_file;

		ss >> path_to_file;
		this->_access_controller.refresh_tmp_map_access_level(path_to_file);
		this->_access_controller.apply_tmp_map_access_level();
	}

	// -- setting -- setting -- setting -- setting --
	else if (order == Constant::Comunicate::setting_changed) {
		std::cerr << "sas?\n";
		if (!this->_setting_controller.is_setting_chenge())
			this->_try_sendAnswer(Constant::Comunicate::setting_not_delivered, Constant::TCP_IP::listen_port, 3);
			// this->_sendAnswer(Constant::Comunicate::setting_not_delivered, Constant::TCP_IP::listen_port);
		else
			this->_try_sendAnswer(Constant::Comunicate::setting_delivered, Constant::TCP_IP::listen_port, 3);

			// this->_sendAnswer(Constant::Comunicate::setting_delivered, Constant::TCP_IP::listen_port);
	}
	else if (order == Constant::Comunicate::apply_setting) {
		if (this->_setting_controller.apply_setting()) {
			this->_try_sendAnswer(this->_get_answer_message_setting_unapply(), Constant::TCP_IP::listen_port, 30);
		}
		else {
			// this->_setting_controller.save_setting();
			this->_try_sendAnswer(Constant::Comunicate::setting_applyed, Constant::TCP_IP::listen_port, 30);
		}
	}
	else if (order == Constant::Comunicate::setting_rool_back) {
		std::vector<std::string>	list_unapply_setting;
		std::string 				str_unapply_setting;

		getline(ss, str_unapply_setting);
		list_unapply_setting = Parser::custom_split(str_unapply_setting, " ");
		this->_setting_controller.roolback_setting(list_unapply_setting);
	}
	else if (order == Constant::Comunicate::setting_save)
		this->_setting_controller.save_setting();
	// -- setting -- setting -- setting -- setting --

	return true;
}

void 			Client::_try_sendAnswer(std::string message, int port, int nbr_try) {
	int 	count_try = 0;

	while (count_try < nbr_try) {
		if (this->_sendAnswer(message, port) == 0)
			return;
		count_try++;
		sleep(1);
	}
}


void 			Client::_get_key() {

}

void 			Client::_sendSelfInfo() {
	struct json_object	*jobj;
	std::string 		message;

	jobj = this->_info_controller.get_router_info_json();
	message = json_object_get_string(jobj);
	this->_sendAnswer(message, Constant::TCP_IP::listen_port);
}

int 			Client::_sendAnswer(std::string message, int potr) {
	TCP_IP_Worker 			tcp_ip;
	RouterData 				server = this->_info_controller.get_server_info();
	int 					i = 0;

	if (!server.ip.size()){
		std::cerr << "ERROR: this router does not know ip server\n";
		return -1;
	}
	while (tcp_ip.tcp_connect(server.ip, potr, 1)) {
		if (i > 100)
			break ;
		usleep(200);
		i++;
	}
	if (i > 100) {
		std::cerr << "(ANSWER NOT SENDED): " << message << "\n";
		perror("reason: ");
		return -1;
	}
	std::cerr << "success connect, try: " << i << "\n";
	tcp_ip.tcp_send(message);
	return 0;
}


std::string 	Client::_get_answer_message_setting_unapply() {
	std::string 		answer;

	answer += Constant::Comunicate::setting_not_applyed;
	answer += " " + this->_setting_controller.get_str_unapply_options();
	return answer;
}

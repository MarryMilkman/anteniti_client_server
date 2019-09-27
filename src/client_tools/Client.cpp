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
   _blocking_controller(BlockingController::getInstance())
{
	std::cerr << "Client:\n";
	// this->_get_key();
	this->_startWork();
}



void 	Client::_startWork() {

	while (1) {
		this->_listenBroadcast(10);
		// if (!this->_status_controller.server_availabilit) {
		// 	if (this->_setting_controller.is_setting_chenge() || StatusController::isWAN()) {
		// 		throw CustomException(eExceptType::e_need_server_work_mod);
		// 	}
		// }
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
	if (order == SEND_INFO)
		this->_sendSelfInfo();
	else if (order == SERVER_MOD_FREE || order == WAN_CHENGED)
		this->_status_controller.server_availabilit = false;
	else if (order == KEY_WAS_CHANGED)
		this->_get_key();
	else if (order == SEND_SETTING_VERSION) {
		int version = this->_setting_controller.get_version();
		this->_sendAnswer(std::to_string(version), LISTEN_PORT);
	}

	else if (order == BLOCKLIST_CHANGE)
		this->_blocking_controller.apply();

	// -- setting -- setting -- setting -- setting --
	else if (order == SETTING_CHANGED) {
		if (!this->_setting_controller.is_setting_chenge())
			this->_sendAnswer(SETTING_NOT_DELIVERED, LISTEN_PORT);
		else
			this->_sendAnswer(SETTING_DELIVERED, LISTEN_PORT);
	}
	else if (order == SETTING_APPLY) {
		if (this->_setting_controller.apply_setting()) {
			this->_sendAnswer(this->_get_answer_message_setting_unapply(), LISTEN_PORT);
		}
		else {
			this->_setting_controller.save_setting();
			this->_sendAnswer(SETTING_APPLYED, LISTEN_PORT);
		}
	}
	else if (order == SETTING_ROOL_BACK) {
		std::vector<std::string>	list_unapply_setting;
		std::string 				str_unapply_setting;

		getline(ss, str_unapply_setting);
		list_unapply_setting = Parser::custom_split(str_unapply_setting, " ");
		this->_setting_controller.roolback_setting(list_unapply_setting);
	}
	else if (order == SETTING_SAVE)
		this->_setting_controller.save_setting();
	// -- setting -- setting -- setting -- setting --

	return true;
}

void 			Client::_get_key() {

}

void 			Client::_sendSelfInfo() {
	std::string	message;

	message = this->_info_controller.get_info_for_cloud();
	this->_sendAnswer(message, LISTEN_PORT);
}

void 			Client::_sendAnswer(std::string message, int potr) {
	TCP_IP_Worker 			tcp_ip;
	RouterData 				server = this->_info_controller.get_server_info();
	int 					i = 0;

	if (!server.ip.size()){
		std::cerr << "ERROR: this router does not know ip server\n";
		return;
	}
	// std::cerr << server.ip << " alllllllaaah  pomogi!\n";
	while (tcp_ip.tcp_connect(server.ip, potr, 1)) {
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
	tcp_ip.tcp_send(message);
}


std::string 	Client::_get_answer_message_setting_unapply() {
	std::string 		answer;

	answer += SETTING_NOT_APPLYED;
	answer += " " + this->_setting_controller.get_str_unapply_options();
	return answer;
}

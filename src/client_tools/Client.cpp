#include "Client.hpp"
#include "CustomException.hpp"
#include "ScriptExecutor.hpp"

#include "TCP_IP_Worker.hpp"

#include "controllers/RouterInfoController.hpp"
#include "controllers/BroadcastController.hpp"
#include "controllers/SettingController.hpp"
#include "controllers/StatusController.hpp"

#include "Timer.hpp"


Client::~Client(){}

Client::Client()
{
	std::cerr << "Client:\n";
	RouterInfoController::getInstance();
	// this->_get_key();
	this->_startWork();
}



void 	Client::_startWork() {
	int count_emty_broadcast = 0;
	StatusController &s_c = StatusController::getInstance();

	while (1) {
		if (this->_listenBroadcast())
			count_emty_broadcast = 0;
		if (!s_c.server_availabilit || count_emty_broadcast++ > 7) {
			if (SettingController::getInstance().is_setting_chenge() || StatusController::isWAN()) {
				throw CustomException(eExceptType::e_need_server_work_mod);
			}
		}
	}
}

bool 	Client::_listenBroadcast() {
	BroadcastController		&bc_controller = BroadcastController::getInstance();
	StatusController 		&s_c = StatusController::getInstance();
	std::stringstream 		ss;
	std::string				order;

	if (bc_controller.receive(1))
		return false;
	ss << bc_controller.get_message();
	ss >> order;
	std::cerr << "Message geting: *" << order << "*\n";
	s_c.server_availabilit = true;
	if (order == SEND_INFO)
		this->_sendSelfInfo();
	else if (order == SERVER_MOD_FREE || order == WAN_CHENGED)
		s_c.server_availabilit = false;
	else if (order == KEY_WAS_CHANGED)
		this->_get_key();
	else if (order == SEND_SETTING_VERSION)
		this->_sendAnswer("no version", LISTEN_PORT);

	// -- setting -- setting -- setting -- setting --
	else if (order == SETTING_CHENGED) {
		if (!SettingController::getInstance().is_setting_chenge())
			this->_sendAnswer(SETTING_NOT_DELIVERED, LISTEN_PORT);
		else
			this->_sendAnswer(SETTING_DELIVERED, LISTEN_PORT);
	}
	else if (order == SETTING_APPLY) {
		if (SettingController::getInstance().apply_setting())
			this->_sendAnswer(this->_get_answer_message_setting_unapply(), LISTEN_PORT);
		else
			this->_sendAnswer(SETTING_APPLYED, LISTEN_PORT);
	}
	else if (order == SETTING_ROOL_BACK) {
		std::vector<std::string>	list_unapply_setting;
		std::string 				str_unapply_setting;

		getline(ss, str_unapply_setting);
		list_unaplly_setting = custom_split(str_unapply_setting, " ");
		SettingController::getInstance().roolback_setting(list_unaplly_setting);
	}
	else if (order == SETTING_SAVE)
		SettingController::getInstance().save_setting();
	// -- setting -- setting -- setting -- setting --

	return true;
}

void 			Client::_get_key() {

}

void 			Client::_sendSelfInfo() {
	std::string	message;

	message = RouterInfoController::getInstance().get_info_for_cloud();
	this->_sendAnswer(message, LISTEN_PORT);
}

void 			Client::_sendAnswer(std::string message, int potr) {
	TCP_IP_Worker 			tcp_ip;
	RouterData 				server = RouterInfoController::getInstance().get_server_info();
	int 					i = 0;

	if (!server.ip.size()){
		std::cerr << "ERROR: this router does not know ip server\n";
		return;
	}
	// std::cerr << server.ip << " alllllllaaah  pomogi!\n";
	while (tcp_ip.tcp_connect(server.ip, potr, 1)) {
		if (i++ > 100)
			break ;
		usleep(200);
	}
	if (i > 100) {
		std::cerr << "(ANSWER NOT SENDED): " << message << "\n";
		perror("reason: ");
		return ;
	}
	std::cerr << "success connect, try: " << i << "\n";
	tcp_ip.tcp_send(message);
}

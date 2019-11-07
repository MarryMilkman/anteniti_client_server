#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "lib.h"

#include "controllers/RouterInfoController.hpp"
#include "controllers/BroadcastController.hpp"
#include "controllers/SettingController.hpp"
#include "controllers/StatusController.hpp"
#include "controllers/AccessController.hpp"

class Client
{
public:
	Client();
	~Client();

private:
	RouterInfoController	&_info_controller;
	BroadcastController		&_bc_controller;
	StatusController		&_status_controller;
	SettingController		&_setting_controller;
	AccessController		&_access_controller;

	void			_startWork();

	void 			_try_sendAnswer(std::string message, int port, int nbr_try);

	bool 			_listenBroadcast(int timeout);
	int 			_sendAnswer(std::string message, int port);
	void 			_sendSelfInfo();

	std::string 	_get_answer_message_setting_unapply();
	void 			_get_key();

};

#endif

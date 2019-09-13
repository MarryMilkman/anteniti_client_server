#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "lib.h"

class Client
{
public:
	Client();
	~Client();

private:
	void			_startWork();

	bool 			_listenBroadcast(int timeout);
	void 			_sendAnswer(std::string message, int port);
	void 			_sendSelfInfo();

	std::string 	_get_answer_message_setting_unapply();
	void 			_get_key();

};

#endif

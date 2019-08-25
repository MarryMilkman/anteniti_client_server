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

	bool 			_listenBroadcast();
	void 			_sendAnswer(std::string message, int port);
	void 			_sendSelfInfo();

	void 			_get_key();

};

#endif

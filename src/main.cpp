#include "lib.h"

#include "Server.hpp"
#include "Client.hpp"

#include "FlagsHendler.hpp"
#include "controllers/StatusController.hpp"
#include "controllers/SettingController.hpp"
#include "controllers/RouterInfoController.hpp"
#include "controllers/BroadcastController.hpp"
#include "controllers/CloudController.hpp"
#include "controllers/NotificationController.hpp"
#include "controllers/AccessController.hpp"
#include "controllers/ConnectionController.hpp"
#include "Informer.hpp"

#include "ScriptExecutor.hpp"

int 		make_ping(std::string ip) {
	std::string	script = Constant::ScriptExec::script_path + "pingcheck.sh";
	std::string answer_ping;
	int			result = 0;

	answer_ping = ScriptExecutor::getOutput::execute(3, script.c_str(), "1", ip.c_str());
	try {
		result = std::stoi(answer_ping);
	} catch (std::exception &e) {}
	return result;
}


int main(int argc, char const *argv[])
{
	std::string		host;
	int				port;
	int				mod;

	FlagsHendler f = FlagsHendler(argc, argv, host, port, mod);

	CloudController::getInstance();
	StatusController::getInstance();
	SettingController::getInstance();
	RouterInfoController::getInstance();
	BroadcastController::getInstance();
	NotificationController::getInstance();
	ConnectionController 	&_connection_controller = ConnectionController::getInstance();
	std::cerr << "init controllers end\n";

	Informer				informer;

	std::thread 	thread_connection(std::ref(_connection_controller));
	std::thread 	thread_informer(std::ref(informer));

	thread_connection.detach();
	thread_informer.detach();
	// sleep(1);
//////////////////////
	if (mod == 0)
		Server();
	else if (mod == 1)
		Client();
//////////////////////
	while (1) {
		StatusController::getInstance().choiceModWork();
	}
	return 0;
}

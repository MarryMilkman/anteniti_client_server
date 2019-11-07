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

#include <curl/curl.h>

//
// // get nick ip by mac
#include "ScriptExecutor.hpp"

std::map<std::string /*type*/, std::string /*value*/>	get_dev_info_by_mac(std::string mac) {
	std::map<std::string, std::string> 			info_map;
	std::string 		script = Constant::ScriptExec::script_path + "getname.sh";
	std::string 		info;
	std::stringstream	ss;

	info = ScriptExecutor::getOutput::execute(2, script.c_str(), mac);
	ss << info;
	ss >> info_map["ip"];
	ss >> info_map["nick"];
	return info_map;
}


int main(int argc, char const *argv[])
{
	std::string		host;
	int				port;
	int				mod;

	// if (!libssh2_init(0))
		// std::cerr << "libssh2 dosnt init!\n";




	CloudController::getInstance();
	StatusController::getInstance();
	SettingController::getInstance();
	RouterInfoController::getInstance();
	BroadcastController::getInstance();
	NotificationController::getInstance();
	ConnectionController &_connection_controller = ConnectionController::getInstance();

	std::cerr << "init controllers end\n";


	FlagsHendler f = FlagsHendler(argc, argv, host, port, mod);
	std::thread 	thread_notification(std::ref(_connection_controller));
	thread_notification.detach();
	// sleep(1);
//////////////////////
	if (mod == 0)
		Server();
	else if (mod == 1)
		Client();
//////////////////////
	while (1) {
		std::cerr << "hmmmmmmmmmmmmmmmmmmmmmmm\n";
		StatusController::getInstance().choiceModWork();
	}
	return 0;
}

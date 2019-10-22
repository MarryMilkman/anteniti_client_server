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

#include <curl/curl.h>

//
// // split
//
// std::vector<std::string> custom_split(std::string str, std::string delim) {
// 	int 						i;
// 	std::vector<std::string>	r_list;
// 	std::string 				prom_str;
//
//
// 	while (str.size()) {
// 		i = str.find(delim);
// 		if (i < 0) {
// 			if (!str.empty())
// 			r_list.push_back(str);
// 			break;
// 		}
// 		str[i] = 0;
// 		r_list.push_back(str.c_str());
// 		str = (str.c_str() + i + delim.size());
// 	}
// 	// libssh2_exit();
// 	return r_list;
// }

#include "Mutex.hpp"

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
	NotificationController &_notification_controller = NotificationController::getInstance();

	std::cerr << "init controllers end\n";


	FlagsHendler f = FlagsHendler(argc, argv, host, port, mod);
	std::thread 	thread_notification(std::ref(_notification_controller));
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
	if (thread_notification.joinable())
		thread_notification.join();
	return 0;
}

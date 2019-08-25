#include "lib.h"

#include "Server.hpp"
#include "Client.hpp"

#include "FlagsHendler.hpp"
#include "controllers/StatusController.hpp"
#include "controllers/SettingController.hpp"

#include <curl/curl.h>


// split

std::vector<std::string> custom_split(std::string str, std::string delim) {
	int 						i;
	std::vector<std::string>	r_list;
	std::string 				prom_str;

	while (str.size()) {
		i = str.find(delim);
		if (i < 0) {
			if (!str.empty())
			r_list.push_back(str);
			break;
		}
		str[i] = 0;
		r_list.push_back(str.c_str());
		str = (str.c_str() + i + delim.size());
	}
	return r_list;
}

int main(int argc, char const *argv[])
{
	std::string		host;
	int				port;
	int				mod;

	SettingController::getInstance();
	FlagsHendler f = FlagsHendler(argc, argv, host, port, mod);
//////////////////////
	if (mod == 0)
		Server();
	else if (mod == 1)
		Client();
//////////////////////
	while (1)
		StatusController::getInstance().choiceModWork();
	return 0;
}

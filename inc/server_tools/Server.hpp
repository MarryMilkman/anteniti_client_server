#ifndef SERVER_HPP
#define SERVER_HPP


#include "lib.h"


class Server
{
public:
	Server();
	~Server();

private:
	void				_startWork();
	void 				_init();

	void 				_take_on_responsibility();
	bool 				_refresh_setting_in_mesh();
	bool 				_refresh_general_setting_in_mesh();

	// cloud
	void 				_get_new_key_and_notify();
	void 				_sendErrorTo_cloud(std::vector<RouterData> &list_routers);
	int 				_sendSelfInfoTo_cloud();

	// ssh
	int 				_send_setting_to(std::string path_setting, std::vector<RouterData> &list_routers);

	// comunicated (tcp/ip)
	void 				_get_info_from_routers_and_send_to_cloud(
				std::vector<RouterData> list_routers);
	int 				_send_and_notify_setting_chenge(
				std::vector<RouterData> list_routers);
	int 				_order_applay_and_save_setting(
				std::vector<RouterData> list_routers);

	void				_listenAnswers(
				std::vector<RouterData> &list_routers,
				std::string 			answer_success,
				std::string				answer_fail,
				int 					port,
				int 					timeout);


};

#endif

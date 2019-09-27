#ifndef SERVER_HPP
#define SERVER_HPP


#include "lib.h"
#include "controllers/RouterInfoController.hpp"
#include "controllers/BroadcastController.hpp"
#include "controllers/StatusController.hpp"
#include "controllers/SettingController.hpp"
#include "controllers/CloudController.hpp"
#include "controllers/SSHTunnelController.hpp"
#include "controllers/BlockingController.hpp"

class Server
{
public:
	Server();
	~Server();


private:
	RouterInfoController	&_info_controller;
	BroadcastController 	&_bc_controller;
	StatusController 		&_status_controller;
	SettingController 		&_setting_controller;
	CloudController 		&_cloud_controller;
	SSHTunnelController 	&_ssh_tunnel_controller;
	BlockingController		&_blocking_controller;

	static std::mutex 	_mutex;
	std::string 		_error_message;

	void				_startWork();
	void 				_init();

	void 				_take_on_responsibility();
	bool 				_refresh_setting_in_mesh();
	bool 				_refresh_blocklist_in_mesh();
	bool 				_refresh_general_setting_in_mesh();

	void 		_thread_metod();
	// cloud
	void 				_get_new_key_and_notify();
	void 				_form_error_message(std::vector<RouterData> &list_routers);

	// ssh
	int 				_send_setting_to(std::string path_setting, std::vector<RouterData> &list_routers);

	// comunicated (tcp/ip)
	static void 		_start_send_devices_info();

	void 				_get_info_from_routers_and_send_to_cloud(
				std::vector<RouterData> list_routers);
	int 				_send_and_notify_setting_chenge(
				std::string path_setting,
				std::vector<RouterData> list_routers);
	int 				_order_applay_and_save_setting(
				std::vector<RouterData> list_routers);


	static void			_listenAnswers(
				std::vector<RouterData> &list_routers,
				std::string 			answer_success,
				int 					port,
				int 					timeout);




};

#endif

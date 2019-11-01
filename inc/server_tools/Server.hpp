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
#include "controllers/NotificationController.hpp"

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
	NotificationController	&_notification_controller;

	static std::mutex 	_mutex;
	std::string 		_error_message;
	time_t 				_time_last_request;

	void 				_init();

	void 				_take_on_responsibility();
	void				_startWork();
	void 				_handle_instruction(std::string instruction);


	// setting
	bool 				_refresh_setting_in_mesh();
	bool 				_refresh_general_setting_in_mesh();
	int 				_send_setting_to(std::string path_setting, std::vector<RouterData> &list_routers);

	bool 				_refresh_blocklist_in_mesh();

	void 				_thread_metod();

	// void 				_get_new_key_and_notify();

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


	void 				_form_error_message(std::vector<RouterData> &error_list_routers);


};

#endif

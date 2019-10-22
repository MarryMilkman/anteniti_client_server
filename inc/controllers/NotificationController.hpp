#ifndef NOTIFICATION_CONTROLLER_HPP
# define NOTIFICATION_CONTROLLER_HPP

# include "lib.h"
# include "controllers/CloudController.hpp"
# include "controllers/StatusController.hpp"
# include "controllers/RouterInfoController.hpp"

# define CONNECTION_LOG "/tmp/connection_log.txt"
# define NOTIFICATION_PORT 9939


class Event;


class	NotificationController {
	NotificationController();
public:
	NotificationController(NotificationController const &ref) = delete;
	NotificationController operator=(NotificationController const &ref) = delete;
	~NotificationController();
	static NotificationController &getInstance();

	void 					operator()();

	bool 					notify_new_connect;
	bool 					notify_common_connect;
	bool 					notify_disconnect;

private:
	CloudController			&_cloud_controller;
	StatusController 		&_status_controller;
	RouterInfoController 	&_info_controller;


	pid_t 					_watcher_general;
	pid_t 					_watcher_general5;
	pid_t 					_watcher_sump;
	pid_t 					_watcher_sump5;
	pid_t 					_watcher_guest;
	pid_t 					_watcher_guest5;

	std::vector<Event>		_list_event;
	time_t 					_time_last_check;

	void 					_server_behavior();
	void 					_client_behavior();

	  // comunicate
	void 					_listen_sattelites_events(int timeout);
	void 					_send_events_to_master();

	  // logics methods
	void 					_tracking_self_events();
	void 					_explore_and_clean_connection_log();
	void 					_clean_list_from_old_devices();
	void 					_handle_events();
	bool 					_is_time_notify();


	  // wahtcher methods
	void 					_check_watchers();
	void 					_check_watchers_general(int temp_process);
	void 					_check_watchers_sump(int temp_process);
	void 					_check_watchers_guest(int temp_process);


};


#endif

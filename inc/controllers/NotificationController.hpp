#ifndef NOTIFICATION_CONTROLLER_HPP
# define NOTIFICATION_CONTROLLER_HPP

# include "lib.h"
# include "controllers/CloudController.hpp"
# include "controllers/StatusController.hpp"
# include "controllers/RouterInfoController.hpp"

// # define CONNECTION_LOG "/tmp/connection_log.txt"
// # define NOTIFICATION_PORT 9939


class Event;
class ConnectedDeviceInfo;


class	NotificationController {
	NotificationController();
public:
	NotificationController(NotificationController const &ref) = delete;
	NotificationController operator=(NotificationController const &ref) = delete;
	~NotificationController();
	static NotificationController &getInstance();

	void 								operator()();
	void 								refresh_list_familiar_devices();

	bool 								notify_new_connect;
	bool 								notify_common_connect;
	bool 								notify_disconnect;
	bool 								notify_general;
	bool 								notify_guest;
	bool 								notify_sump;

private:
	CloudController						&_cloud_controller;
	StatusController 					&_status_controller;
	RouterInfoController 				&_info_controller;

	std::mutex 							_list_familiar_devices_mutex;

	std::vector<Event>					_list_events;
	std::vector<std::string>			_list_familiar_mac;
	std::vector<ConnectedDeviceInfo>	_list_connected_devices;
	time_t 								_time_last_check;

		// Watchers process id
	pid_t 								_watcher_general;
	pid_t 								_watcher_general5;
	pid_t 								_watcher_sump;
	pid_t 								_watcher_sump5;
	pid_t 								_watcher_guest;
	pid_t 								_watcher_guest5;

		// behavior
	void 								_server_behavior();
	void 								_client_behavior();

	  // comunicate
	void 								_listen_sattelites_events(int timeout);
	void 								_send_events_to_master();

	  // logics methods
	void 								_tracking_self_events();
	void 								_explore_and_clean_connection_log();
	void 								_handle_events();
	bool 								_is_time_notify();
	bool 								_is_jamming();
	  // // filter
	void 								_filter_list_events();
	void 								_check_fame(Event & event);
	void 								_consistency_check_of_events();



	  // wahtcher methods
	void 								_check_watchers();
	void 								_check_watchers_general(int temp_process);
	void 								_check_watchers_sump(int temp_process);
	void 								_check_watchers_guest(int temp_process);
	pid_t 								_make_watcher(std::string script);


};


#endif

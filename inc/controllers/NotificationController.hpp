#ifndef NOTIFICATION_CONTROLLER_HPP
# define NOTIFICATION_CONTROLLER_HPP

# include "lib.h"
# include "controllers/CloudController.hpp"
# include "controllers/StatusController.hpp"
# include "controllers/RouterInfoController.hpp"

// # define CONNECTION_LOG "/tmp/connection_log.txt"
// # define NOTIFICATION_PORT 9939


class EventConnect;
class ConnectedDeviceInfo;


class	NotificationController {
	NotificationController();
public:
	NotificationController(NotificationController const &ref) = delete;
	NotificationController operator=(NotificationController const &ref) = delete;
	~NotificationController();
	static NotificationController &getInstance();

	void 								handle_events(std::vector<EventConnect> list_events);
	// void 								refresh_list_familiar_devices();

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

	std::vector<EventConnect>			_list_events;
	std::vector<ConnectedDeviceInfo>	_list_connected_devices;

	bool 								_is_time_notify();
	bool 								_is_jamming();

	void 								_filter_list_events();
	void 								_consistency_check_of_events();
};


#endif

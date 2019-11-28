#ifndef CONNECTION_CONTROLLER
# define CONNECTION_CONTROLLER

# include "lib.h"
# include "controllers/StatusController.hpp"
# include "controllers/RouterInfoController.hpp"
# include "controllers/NotificationController.hpp"
# include "controllers/AccessController.hpp"
# include "controllers/BroadcastController.hpp"

class EventConnect;

class ConnectionController {
	ConnectionController();
public:
	ConnectionController(ConnectionController const &ref) = delete;
	ConnectionController &operator=(ConnectionController const &ref) = delete;
	~ConnectionController();
	static ConnectionController		&getInstance();

	void 							operator()();

private:
	StatusController 				&_status_controller;
	RouterInfoController 			&_info_controller;
	NotificationController 			&_notification_controller;
	AccessController 				&_access_controller;
	BroadcastController				&_bc_controller;

	std::vector<EventConnect>				_list_events;
		// Watchers process id
	pid_t 							_watcher_general;
	pid_t 							_watcher_general5;
	pid_t 							_watcher_sump;
	pid_t 							_watcher_sump5;
	pid_t 							_watcher_guest;
	pid_t 							_watcher_guest5;

		// behavior
	void							_server_behavior();
	void							_client_behavior();

	void 							_listen_sattelites_events(int timeout);
	void 							_send_events_to_master(std::vector<EventConnect> list_events_for_notify);

	void 							_tracking_self_events();
		// returns list events which have lease
	std::vector<EventConnect>		_handl_connection();
	void 							_explore_and_clean_connection_log();


		// wahtcher methods
	void 							_check_watchers();
	void 							_check_watchers_general(int temp_process);
	// void 							_check_watchers_sump(int temp_process);
	// void 							_check_watchers_guest(int temp_process);
	pid_t 							_make_watcher(std::string script);
};

#endif

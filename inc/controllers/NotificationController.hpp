#ifndef NOTIFICATION_CONTROLLER_HPP
# define NOTIFICATION_CONTROLLER_HPP

# include "lib.h"

class NotificationController {
	NotificationController();
public:
	NotificationController(NotificationController const &ref) = delete;
	NotificationController operator=(NotificationController const &ref) = delete;
	~NotificationController();
	static NotificationController &getInstance();

	void 		check_and_send();

private:
	void 		_check_watchers();

	void 		_check_watchers_general(int temp_process);
	void 		_check_watchers_sump(int temp_process);
	void 		_check_watchers_guest(int temp_process);

	pid_t 		_watcher_general;
	pid_t 		_watcher_general5;

	pid_t 		_watcher_sump;
	pid_t 		_watcher_sump5;

	pid_t 		_watcher_guest;
	pid_t 		_watcher_guest5;

};


#endif

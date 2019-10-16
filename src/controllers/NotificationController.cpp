#include "controllers/NotificationController.hpp"
#include "ScriptExecutor.hpp"

NotificationController::NotificationController() {
	this->_watcher_general = 0;
	this->_watcher_general5 = 0;

	this->_watcher_sump = 0;
	this->_watcher_sump5 = 0;

	this->_watcher_guest = 0;
	this->_watcher_guest5 = 0;

}

NotificationController::~NotificationController() {
}

NotificationController &NotificationController::getInstance() {
    static NotificationController  _notification_controller;

    return _notification_controller;
}

void 		NotificationController::check_and_send() {
	std::cerr << "hello!\n";
	_check_watchers();
	std::cerr << "pesda tvoemu logu!\n";
}







void 		NotificationController::_check_watchers() {
	std::string 	script = SCRIPT_PATH "testconn.sh";
	int 			options = WNOHANG;
	pid_t 			term_process = waitpid(-1, 0, options);

	if (term_process == 0)
		return ;
	this->_check_watchers_general(term_process);
	this->_check_watchers_sump(term_process);
	this->_check_watchers_guest(term_process);
}

void 		NotificationController::_check_watchers_general(int term_process) {
	std::string 	script = SCRIPT_PATH "testconn.sh";

	if (this->_watcher_general <= 0 || this->_watcher_general == term_process) {
		pid_t	new_pid = fork();
		if (!new_pid) {
			script = std::string("hostapd_cli -i wlan0 -a ") + script;
			system(script.c_str());
			exit(0);
		}
		this->_watcher_general = new_pid;
	}
	if (this->_watcher_general5 <= 0 || this->_watcher_general5 == term_process) {
		pid_t	new_pid = fork();
		if (!new_pid) {
			script = std::string("hostapd_cli -i wlan1-1 -a ") + script;
			system(script.c_str());
			exit(0);
		}
		this->_watcher_general5 = new_pid;
	}
}

void 		NotificationController::_check_watchers_sump(int term_process) {
	std::string 	script = SCRIPT_PATH "testconn.sh";

	if (this->_watcher_sump <= 0 || this->_watcher_sump == term_process) {
		pid_t	new_pid = fork();
		if (!new_pid) {
			script = std::string("hostapd_cli -i wlan0-2 -a ") + script;
			system(script.c_str());
			exit(0);
		}
		this->_watcher_sump = new_pid;
	}
	if (this->_watcher_sump5 <= 0 || this->_watcher_sump5 == term_process) {
		pid_t	new_pid = fork();
		if (!new_pid) {
			script = std::string("hostapd_cli -i wlan1-2 -a ") + script;
			system(script.c_str());
			exit(0);
		}
		this->_watcher_sump5 = new_pid;
	}
}

void 		NotificationController::_check_watchers_guest(int term_process) {
	std::string 	script = SCRIPT_PATH "testconn.sh";

	if (this->_watcher_guest <= 0 || this->_watcher_guest == term_process) {
		pid_t	new_pid = fork();
		if (!new_pid) {
			script = std::string("hostapd_cli -i wlan0-3 -a ") + script;
			system(script.c_str());
			exit(0);
		}
		this->_watcher_guest = new_pid;
	}
	if (this->_watcher_guest5 <= 0 || this->_watcher_guest5 == term_process) {
		pid_t	new_pid = fork();
		if (!new_pid) {
			script = std::string("hostapd_cli -i wlan1-3 -a ") + script;
			system(script.c_str());
			exit(0);
		}
		this->_watcher_guest5 = new_pid;
	}
}

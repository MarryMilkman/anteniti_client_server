#include "controllers/NotificationController.hpp"
#include "EventConnect.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "ScriptExecutor.hpp"
#include "Timer.hpp"
#include "Parser.hpp"
#include "TCP_IP_Worker.hpp"

#include <cerrno>

NotificationController::NotificationController() :
	_cloud_controller(CloudController::getInstance()),
	_status_controller(StatusController::getInstance()),
	_info_controller(RouterInfoController::getInstance())
{
	std::fstream 	file;

	this->notify_new_connect = true;
	this->notify_common_connect = true;
	this->notify_disconnect = true;
	this->notify_general = true;
	this->notify_guest = true;
	this->notify_sump = true;

	this->_list_connected_devices = RouterInfoController::get_list_connected_devices();
}

NotificationController::~NotificationController() {
}

NotificationController &NotificationController::getInstance() {
    static NotificationController  _notification_controller;

    return _notification_controller;
}


// depending on setting, notificat about event or ignore it
void 		NotificationController::handle_events(std::vector<EventConnect> list_events) {
   this->_list_events = list_events;
   this->_filter_list_events();
   if (!this->_is_time_notify() || !this->_list_events.size())
	   return ;
   for (EventConnect event : this->_list_events) {
	   std::cerr << event.get_str() << "from NotificationController::_handle_events\n";
	   if (event.conn && this->notify_new_connect && event.is_new) {
		   this->_cloud_controller.notificat(Constant::Cloud::notif_connect_new_d, event.nick);
		   continue;
	   }
	   if (event.conn && this->notify_common_connect) {
		   this->_cloud_controller.notificat(Constant::Cloud::notif_connect_d, event.nick);
		   continue;
	   }
	   if (!event.conn && this->notify_disconnect)
		   this->_cloud_controller.notificat(Constant::Cloud::notif_disconnect_d , event.nick);
   }

   this->_list_connected_devices = RouterInfoController::get_list_connected_devices();
   // this->_list_events.clear();
}

void 		NotificationController::_filter_list_events() {
	for (int i = 0, size = this->_list_events.size(); i < size;) {
		EventConnect &event = this->_list_events[i];

		// this->_check_fame(event);
		if (!this->notify_general && (event.iface == "wlan0" || event.iface == "wlan1-2")) {
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			continue;
		}
		if (!this->notify_guest && (event.iface == "wlan0-3" || event.iface == "wlan1-4")) {
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			continue;
		}
		if (!this->notify_sump && (event.iface == "wlan0-2" || event.iface == "wlan1-3")) {
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			continue;
		}
		i++;
	}
	this->_consistency_check_of_events();
}

void 		NotificationController::_consistency_check_of_events() {
	std::vector<ConnectedDeviceInfo>	fresh_list_connected_devices = RouterInfoController::get_list_connected_devices();
	bool 								need_reload_list = false;

	for (int i = 0, size = this->_list_events.size(); i < size;) {
		EventConnect	&event = this->_list_events[i];
		bool 	fresh_list_is_exist = false;
		bool 	self_list_is_exist = false;

		for (ConnectedDeviceInfo conn_dev : this->_list_connected_devices) {
			if (conn_dev._mac == event.mac) {
				self_list_is_exist = true;
				break ;
			}
		}
		if ((event.conn && self_list_is_exist) /*|| (!event.conn && !self_list_is_exist)*/) {
			std::cerr << "False event: old list\n";

			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			need_reload_list = true;
			continue;
		}
		for (ConnectedDeviceInfo conn_dev : fresh_list_connected_devices) {
			if (conn_dev._mac == event.mac) {
				fresh_list_is_exist = true;
				break ;
			}
		}
		if ((!event.conn && fresh_list_is_exist) /*|| (event.conn && !fresh_list_is_exist)*/) {
			std::cerr << "False event: fresh list\n";
			this->_list_events.erase(this->_list_events.begin() + i);
			size = this->_list_events.size();
			need_reload_list = true;
			continue;
		}
		i++;
	}
	if (need_reload_list)
		this->_list_connected_devices = fresh_list_connected_devices;
}


bool 		NotificationController::_is_time_notify() {
	time_t 	time_reload = this->_status_controller.get_time_reload();
	time_t 	now;

	time(&now);
	if (now - 4 > time_reload)
		return true;
	return false;
}

bool 		NotificationController::_is_jamming() {
	if (!this->_is_time_notify() || !this->_list_events.size())
		return false;

	int						count_dis = 0;
	int 					count_conn = 0;
	std::vector<RouterData>	list_routers;
	std::string 			script = Constant::ScriptExec::script_path + "get_nbr_conn_dev.sh";
	std::string 			answer;

	this->_info_controller.refresh();
	list_routers = this->_info_controller.get_routers_info();
	answer = ScriptExecutor::getOutput::execute(1, script.c_str());
	if (answer != "0\n" || list_routers.size())
		return false;
	for (EventConnect event : this->_list_events) {
		if (event.conn)
			count_conn++;
		else
			count_dis++;
	}
	if (count_conn || count_dis < Constant::Notification::critical_number_dis)
		return false;
	return true;
}

#include "controllers/notification_tools/Event.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"


Event::Event() {

}

Event::Event(std::string str_event) {
	std::stringstream 	pars_ss(str_event);
	std::string 		word;

	pars_ss >> word;
	this->conn = word == "1" ? true : false;
	pars_ss >> this->mac;
	pars_ss >> this->iface;
	pars_ss >> word;
	try {
		this->event_time = std::stoi(word);
	} catch (std::exception & e) {}
	ConnectedDeviceInfo device;

	device._mac = this->mac;
	device.set_nick_ip();
	this->nick = device._nick;
	this->is_new = false;
}

Event::~Event() {}

Event::Event(Event const & ref) {
	*this = ref;
}

Event	&Event::operator=(Event const & ref) {
	this->conn = ref.conn;
	this->mac = ref.mac;
	this->iface = ref.iface;
	this->event_time = ref.event_time;
	this->nick = ref.nick;
	this->is_new = ref.is_new;
	return *this;
}

std::string 	Event::get_str() const {
	std::stringstream ss;

	ss << this->conn << " " << this->mac << " " << this->nick << " "
		<< this->iface << " " << this->event_time << this->is_new << "\n";
	return ss.str();
}

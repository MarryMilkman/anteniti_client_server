#include "EventConnect.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "Parser.hpp"

EventConnect::EventConnect() {
}

EventConnect::EventConnect(std::string str_event) {
	{
		std::vector<std::string> 	list_str_param = Parser::custom_split(str_event, " ");

		if (list_str_param.size() == 6) {
			this->conn = list_str_param[0] == "1" ? true : false;
			this->mac = list_str_param[1];
			this->nick = list_str_param[2];
			this->iface = list_str_param[3];
			try {
				this->event_time = std::stoi(list_str_param[4]);
			} catch (std::exception & e) {
				this->event_time = 0;
			}
			this->is_new = list_str_param[5] == "1" ? true : false;
			return ;
		}
	}

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

EventConnect::~EventConnect() {}

EventConnect::EventConnect(EventConnect const & ref) {
	*this = ref;
}

EventConnect	&EventConnect::operator=(EventConnect const & ref) {
	this->conn = ref.conn;
	this->mac = ref.mac;
	this->iface = ref.iface;
	this->event_time = ref.event_time;
	this->nick = ref.nick;
	this->is_new = ref.is_new;
	return *this;
}

std::string 	EventConnect::get_str() const {
	std::stringstream ss;

	ss << this->conn << " " << this->mac << " " << this->nick << " "
		<< this->iface << " " << this->event_time << " " << this->is_new << "\n";
	return ss.str();
}

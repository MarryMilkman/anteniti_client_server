#include "EventConnect.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "controllers/RouterInfoController.hpp"
#include "Parser.hpp"

EventConnect::EventConnect() {
}

EventConnect::EventConnect(std::string str_event) {
	{
		// costil for client event (get by tcp_ip)
		std::vector<std::string> 	list_str_param = Parser::custom_split(str_event, " ");

		if (list_str_param.size() == 7) {
			this->conn = list_str_param[0] == "1" ? true : false;
			this->mac = list_str_param[1];
			this->iface = list_str_param[2];
			try {
				this->event_time = std::stoi(list_str_param[3]);
			} catch (std::exception & e) {
				this->event_time = 0;
			}
			this->nick = list_str_param[4];
			this->ip = list_str_param[5];
			this->is_new = list_str_param[6] == "1" ? true : false;
			this->is_self = false;
			return ;
		}
	}
	this->is_self = true;
	this->is_new = false;

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
	this->refresh_nick_ip();

}

EventConnect::~EventConnect() {}

EventConnect::EventConnect(EventConnect const & ref) {
	*this = ref;
}

EventConnect	&EventConnect::operator=(EventConnect const & ref) {
	this->conn = ref.conn;
	this->is_new = ref.is_new;
	this->is_self = ref.is_self;
	this->mac = ref.mac;
	this->ip = ref.ip;
	this->nick = ref.nick;
	this->iface = ref.iface;
	this->event_time = ref.event_time;
	return *this;
}

void 			EventConnect::refresh_nick_ip() {
	std::map<std::string, std::string> info_map = RouterInfoController::get_dev_info_by_mac(this->mac);

	this->nick = info_map["nick"];
	this->ip = info_map["ip"];
	if (this->nick.empty())
		this->nick = "unknown";
}

std::string 	EventConnect::get_str() const {
	std::stringstream ss;

	ss << this->conn << " " << this->mac << " " << this->iface << " " << this->event_time << " "
			<< this->nick << " " << this->ip << " " << this->is_new << "\n";
	return ss.str();
}

#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "ScriptExecutor.hpp"

ConnectedDeviceInfo::ConnectedDeviceInfo(int i) {
    this->_nick = "Dev " + std::to_string(i);
    this->_group = "pam_pam";
    this->_typeConn = "WIFI";
    this->_type802 = "5G";
    this->_signal = 88;
    this->_timeWork = "10:10";
    this->_location = "unknown";
    this->_hostName = "host";
    this->_ip = "192.168.1.1" + std::to_string(i);
    this->_mac = "123412341234567" + std::to_string(i);
    this->_isBlocked = false;
}

ConnectedDeviceInfo::ConnectedDeviceInfo() {
}

ConnectedDeviceInfo::ConnectedDeviceInfo(ConnectedDeviceInfo const &ref) {
    *this = ref;
}

ConnectedDeviceInfo &ConnectedDeviceInfo::operator=(ConnectedDeviceInfo const &ref) {
    this->_nick = ref._nick;
    this->_group = ref._group;
    this->_typeConn = ref._typeConn;
    this->_type802 = ref._type802;
    this->_signal = ref._signal;
    this->_timeWork = ref._timeWork;
    this->_location = ref._location;
    this->_hostName = ref._hostName;
    this->_ip = ref._ip;
    this->_mac = ref._mac;
    this->_isBlocked = ref._isBlocked;
	this->_inactiveTime = ref._inactiveTime;
	return *this;
}

ConnectedDeviceInfo::~ConnectedDeviceInfo() {
}

struct json_object 	*ConnectedDeviceInfo::get_json_info() {
	struct json_object	*r_json = json_object_new_object();

	json_object_object_add(r_json, "IP", json_object_new_string(this->_ip.c_str()));
	json_object_object_add(r_json, "MAC", json_object_new_string(this->_mac.c_str()));
	json_object_object_add(r_json, "Signal", json_object_new_string(std::to_string(this->_signal).c_str()));
	json_object_object_add(r_json, "Nick", json_object_new_string(this->_nick.c_str()));
	// json_object_object_add(r_json, "Group", json_object_new_string(this->_group.c_str()));
	json_object_object_add(r_json, "TypeConn", json_object_new_string(this->_typeConn.c_str()));
	json_object_object_add(r_json, "Type802", json_object_new_string(this->_type802.c_str()));
	json_object_object_add(r_json, "TimeWork", json_object_new_string(this->_timeWork.c_str()));
	// json_object_object_add(r_json, "InactiveTime", json_object_new_string(this->_inactiveTime.c_str()));
	// json_object_object_add(r_json, "Location", json_object_new_string(this->_location.c_str()));
	// json_object_object_add(r_json, "HostName", json_object_new_string(this->_hostName.c_str()));
	// json_object_object_add(r_json, "Blocked", json_object_new_string(std::string(this->_isBlocked ? "yes" : "no").c_str()));
	return r_json;
}

std::string     ConnectedDeviceInfo::get_string_info() {
    std::string     info;
    // int             i = 1;

	info += "IP: " + this->_ip + "IP_end ";
	info += "MAC: " + this->_mac + "MAC_end ";
	info += "Signal: " + std::to_string(this->_signal) + "Signal_end ";
    info += "Nick: " + this->_nick + "Nick_end ";
    // info += "Group: " + this->_group + " ";
    // info += "TypeConn: " + this->_typeConn + " ";
    info += "Type802: " + this->_type802 + "Type802_end ";
    info += "TimeWork: " + this->_timeWork + "TimeWork_end ";
	info += "InactiveTime: " + this->_inactiveTime + "InactiveTime_end ";
    // info += "Location: " + this->_location + " ";
    // info += "HostName: " + this->_hostName + " ";
    // info += "Blocked: " + std::string(this->_isBlocked ? "yes" : "no") + " ";
    return info;
}


void 			ConnectedDeviceInfo::set_nick_ip() {
	std::string 		script = Constant::ScriptExec::script_path + "getname.sh";
	std::string 		info;
	std::stringstream	ss;

	info = ScriptExecutor::getOutput::execute(2, script.c_str(), this->_mac.c_str());
	ss << info;
	ss >> this->_ip;
	ss >> this->_nick;
	if (!this->_nick.size())
		this->_nick = "unknown";
}


///////////////////////////////////////////////

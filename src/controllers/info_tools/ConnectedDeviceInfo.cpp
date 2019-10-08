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

std::string     ConnectedDeviceInfo::get_string_info() {
    std::string     info;
    // int             i = 1;

	info += "IP: " + this->_ip + "IP_end\n";
	info += "MAC: " + this->_mac + "MAC_end\n";
	info += "Signal: " + std::to_string(this->_signal) + "Signal_end\n";
    info += "Nick: " + this->_nick + "Nick_end\n";
    // info += "Group: " + this->_group + "\n";
    // info += "TypeConn: " + this->_typeConn + "\n";
    info += "Type802: " + this->_type802 + "Type802_end\n";
    info += "TimeWork: " + this->_timeWork + "TimeWork_end\n";
	info += "InactiveTime: " + this->_inactiveTime + "InactiveTime_end\n";
    // info += "Location: " + this->_location + "\n";
    // info += "HostName: " + this->_hostName + "\n";
    // info += "Blocked: " + std::string(this->_isBlocked ? "yes" : "no") + "\n";
    return info;
}


void 			ConnectedDeviceInfo::set_nick_ip() {
	std::string 		script = SCRIPT_PATH "getname.sh";
	std::string 		info;
	std::stringstream	ss;

	info = ScriptExecutor::getOutput::execute(2, script.c_str(), this->_mac.c_str());
	ss << info;
	ss >> this->_ip;
	ss >> this->_nick;
}


///////////////////////////////////////////////

#include "controllers/info_tools/ConnectedDeviceInfo.hpp"

ConnectedDeviceInfo::ConnectedDeviceInfo(int i) {
    this->_nik = "Dev " + std::to_string(i);
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
    this->_nik = ref._nik;
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

	info += "MAC: " + this->_mac + "\n";
	info += "Signal: " + std::to_string(this->_signal) + "\n";
    // info += "Nik: " + this->_nik + "\n";
    // info += "Group: " + this->_group + "\n";
    // info += "TypeConn: " + this->_typeConn + "\n";
    info += "Type802: " + this->_type802 + "\n";
    info += "TimeWork: " + this->_timeWork + "\n";
	info += "InactiveTime: " + this->_inactiveTime + "\n";
    // info += "Location: " + this->_location + "\n";
    // info += "HostName: " + this->_hostName + "\n";
    // info += "IP: " + this->_ip + "\n";
    // info += "Blocked: " + std::string(this->_isBlocked ? "yes" : "no") + "\n";
    return info;
}



///////////////////////////////////////////////

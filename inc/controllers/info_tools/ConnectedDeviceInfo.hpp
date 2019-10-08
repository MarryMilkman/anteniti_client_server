#ifndef CONNECTED_DEVICE_INFO
#define CONNECTED_DEVICE_INFO

#include "lib.h"

class ConnectedDeviceInfo {
public:

    ConnectedDeviceInfo(int i);

    ConnectedDeviceInfo();
    ConnectedDeviceInfo(ConnectedDeviceInfo const &ref);
    ConnectedDeviceInfo &operator=(ConnectedDeviceInfo const &ref);
    ~ConnectedDeviceInfo();

    std::string     get_string_info();
	void 			set_nick_ip();


    std::string     _nick;
    std::string     _group;
    std::string     _typeConn;
    std::string     _type802;
    int             _signal;
    std::string     _timeWork;
	std::string 	_inactiveTime;
    std::string     _location;
    std::string     _hostName;
    std::string     _ip;
    std::string     _mac;
    bool            _isBlocked;
private:
};

#endif

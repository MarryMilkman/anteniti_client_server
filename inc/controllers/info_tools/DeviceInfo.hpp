#ifndef DEVICE_INFO
#define DEVICE_INFO

#include "lib.h"
#include "RouterData.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"

class DeviceInfo {
public:
    DeviceInfo(RouterData rd);
    DeviceInfo();
    DeviceInfo(DeviceInfo const &ref);
    DeviceInfo &operator=(DeviceInfo const &ref);
    ~DeviceInfo();

    std::string     get_string_info();

    bool            _status; // off || on
    int             _signal;
    std::string     _location;
    bool            _diod;
    std::string     _os; // operation system
    std::string     _model; //model
    std::string     _sn; //serial number
    std::string     _ip;
    std::string     _etherA; // MAC
    std::string     _etherB; // MAC
    std::string     _wifiA; // MAC
    std::string     _wifiB; // MAC

    std::vector<ConnectedDeviceInfo> _list_connected_devices;

private:
	void 			_refresh();

    void            _init_self_info();

    //paaaaaaaaaaaaaaaaaaaaaaaaaaaaars
    //more paaaaaaaars

    void            _init_list_devices();

    //paaaaaaaars again
    //and araaaaaaaaaaain

};

#endif

#include "controllers/info_tools/DeviceInfo.hpp"
#include "Parser.hpp"
#include "ScriptExecutor.hpp"

DeviceInfo::DeviceInfo(RouterData rd) {
	this->_status = true;
    this->_signal = 90;
    this->_location = "unknown";
    this->_diod = false;
    this->_ip = rd.ip;
	this->_refresh();
}

DeviceInfo::DeviceInfo() {
}

DeviceInfo::DeviceInfo(DeviceInfo const &ref) {
    *this = ref;
}

DeviceInfo &DeviceInfo::operator=(DeviceInfo const &ref) {
    this->_status = ref._status;
    this->_signal = ref._signal;
    this->_location = ref._location;
    this->_diod = ref._diod;
    this->_os = ref._os;
    this->_model = ref._model;
    this->_sn = ref._sn;
    this->_ip = ref._ip;
    this->_etherA = ref._etherA;
    this->_etherB = ref._etherB;
    this->_wifiA = ref._wifiA;
    this->_wifiB = ref._wifiB;

    this->_list_connected_devices.clear();
    for (ConnectedDeviceInfo d : ref._list_connected_devices)
        this->_list_connected_devices.push_back(d);
	return *this;
}

DeviceInfo::~DeviceInfo() {
}

std::string     DeviceInfo::get_string_info() {
    std::string     info = "";
    int             i = 1;

	this->_refresh();
    info += "DeviceRouterInfoBegin\n";
    info += "Status " + std::string(this->_status ? "on" : "off") + "\n";
    info += "Signal " + std::to_string(this->_signal) + "%\n";
    info += "Location " + this->_location + "\n";
    info += "Diod " + std::string(this->_diod ? "on" : "off") + "\n";
    info += "OS " + this->_os + "\n";
    info += "Model " + this->_model + "\n";
    info += "SN " + this->_sn + "\n";
    info += "IP " + this->_ip + "\n";
    info += "EtherA " + this->_etherA + "\n";
    info += "EtherB " + this->_etherB + "\n";
    info += "WifiA " + this->_wifiA + "\n";
    info += "WifiB " + this->_wifiB + "\n";
    info += "DeviceRouterInfoEnd\n\n";
    for (ConnectedDeviceInfo dci : this->_list_connected_devices) {
        info += "DeviceDeviceBegin" + std::to_string(i) + "\n";
        info += dci.get_string_info();
        info += "DeviceDeviceEnd" + std::to_string(i++) + "\n";
    }
    return info;
}

void 		DeviceInfo::_refresh() {
	std::string		str_sysinfo = ScriptExecutor::getOutput::execute(1, "/root/sysinfo.sh");
	std::map<std::string, std::string>	map_sysinfo = Parser::Info::pars_sysinfo(str_sysinfo);

    this->_os = map_sysinfo["OS"];
    this->_model = map_sysinfo["ModelName"];
    this->_sn = map_sysinfo["SN"];
    this->_etherA = map_sysinfo["EtherA"];
    this->_etherB = map_sysinfo["EtherB"];
    this->_wifiA = map_sysinfo["WiFiA"];
    this->_wifiB = map_sysinfo["WiFiB"];
}


///////////////////////////////////////////////

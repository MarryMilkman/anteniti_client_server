#include "controllers/info_tools/DeviceInfo.hpp"
#include "Parser.hpp"
#include "ScriptExecutor.hpp"

DeviceInfo::DeviceInfo(RouterData rd) {
	this->_status = true;
    this->_signal = 90;
    this->_location = "unknown";
    this->_diod = false;
    this->_ip = rd.ip;
	this->_sn = rd.serial_number;
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
    info += "DeviceRouterInfoBegin ";
    // info += "Status: " + std::string(this->_status ? "on" : "off") + " ";
    // info += "Signal: " + std::to_string(this->_signal) + "% ";
    // info += "Location: " + this->_location + " ";
    // info += "Diod: " + std::string(this->_diod ? "on" : "off") + " ";
	info += "SN: " + this->_sn + "SN_end ";
	info += "IP: " + this->_ip + "IP_end ";
    info += "OS: " + this->_os + "OS_end ";
    info += "Model: " + this->_model + "Model_end ";
    info += "EtherA: " + this->_etherA + "EtherA_end ";
    info += "EtherB: " + this->_etherB + "EtherB_end ";
    info += "WifiA: " + this->_wifiA + "WifiA_end ";
    info += "WifiB: " + this->_wifiB + "WifiB_end ";
    info += "DeviceRouterInfoEnd  ";
    for (ConnectedDeviceInfo dci : this->_list_connected_devices) {
        info += "DeviceBegin" + std::to_string(i) + " ";
        info += dci.get_string_info();
        info += "DeviceEnd" + std::to_string(i++) + " ";
    }
    return info;
}

void 		DeviceInfo::_refresh() {
	std::string 						script = SCRIPT_PATH "sysinfo.sh";
	std::string							str_sysinfo = ScriptExecutor::getOutput::execute(1, script.c_str());
	std::map<std::string, std::string>	map_sysinfo = Parser::Info::pars_sysinfo(str_sysinfo);

    this->_os = map_sysinfo["OS"];
    this->_model = map_sysinfo["ModelName"];
    // this->_sn = map_sysinfo["SN"];
    this->_etherA = map_sysinfo["EtherA"];
    this->_etherB = map_sysinfo["EtherB"];
    this->_wifiA = map_sysinfo["WiFiA"];
    this->_wifiB = map_sysinfo["WiFiB"];
}


///////////////////////////////////////////////

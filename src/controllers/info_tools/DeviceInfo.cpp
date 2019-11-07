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

struct json_object 	*DeviceInfo::get_json_info() {
	struct json_object	*r_json = json_object_new_object();
	struct json_object	*arr_conn_dev_json = json_object_new_array();

	json_object_object_add(r_json, "SN", json_object_new_string(this->_sn.c_str()));
	json_object_object_add(r_json, "IP", json_object_new_string(this->_ip.c_str()));
	json_object_object_add(r_json, "OS", json_object_new_string(this->_os.c_str()));
	json_object_object_add(r_json, "Model", json_object_new_string(this->_model.c_str()));
	json_object_object_add(r_json, "EtherA", json_object_new_string(this->_etherA.c_str()));
	json_object_object_add(r_json, "EtherB", json_object_new_string(this->_etherB.c_str()));
	json_object_object_add(r_json, "WifiA", json_object_new_string(this->_wifiA.c_str()));
	json_object_object_add(r_json, "WifiB", json_object_new_string(this->_wifiB.c_str()));
	json_object_object_add(r_json, "DEVICE", arr_conn_dev_json);
	for (ConnectedDeviceInfo dci : this->_list_connected_devices)
		json_object_array_add(arr_conn_dev_json, dci.get_json_info());
	return r_json;
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
	std::string 						script = Constant::ScriptExec::script_path + "sysinfo.sh";
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

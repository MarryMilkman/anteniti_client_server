#include "controllers/RouterInfoController.hpp"
#include "ScriptExecutor.hpp"
#include "controllers/info_tools/DeviceInfo.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "Mutex.hpp"
#include "Parser.hpp"

RouterInfoController::RouterInfoController() {
}

RouterInfoController::~RouterInfoController() {}

RouterInfoController &RouterInfoController::getInstance() {
    Lock    lock(RouterInfoController::_mutex, "RouterInfoController");
    static RouterInfoController info_controller;

    info_controller.refresh();
    return info_controller;
}

void                        RouterInfoController::refresh() {
	// need write scripts for take info from system about routers (DHCP)
    RouterData			router("", "root", "11111111", "");
	std::string 		script = SCRIPT_PATH "self_mac_ip.sh";
	std::stringstream 	ss_self_mac_ip(ScriptExecutor::getOutput::execute(1, script.c_str()));
	std::string			line;

	ss_self_mac_ip >> router.serial_number;
	ss_self_mac_ip >> router.ip;

	this->_self_info = router;
	this->refresh_satellites_list();
    // std::string line = ScriptExecutor::getOutput::execute(1, "/tmp/scripts/some.sh");
    // if (line == "ubuntu_general\n") {
    //     this->_self_info = router_ubuntu_general;
    // }
    // else
    //     this->_self_info = router_kube;
    // this->_list_routers.clear();
    // if (this->_self_info.ip != router_kube.ip)
    //     this->_list_routers.push_back(router_kube);
    // // if (this->_self_info.ip != router_ubuntu_satelit.ip)
    // //     this->_list_routers.push_back(router_ubuntu_satelit);
    // if (this->_self_info.ip != router_ubuntu_general.ip)
    //     this->_list_routers.push_back(router_ubuntu_general);
}

void 						RouterInfoController::refresh_satellites_list() {
	std::string 		script = SCRIPT_PATH "satellites_list.sh";
	std::string			string_satelits = ScriptExecutor::getOutput::execute(1, script.c_str());
	std::stringstream 	ss_satellites(string_satelits);
	std::string 		line;

	if (!string_satelits.size())
		return;
	this->_list_routers.clear();
	while (getline(ss_satellites, line)) {
		RouterData 			n_router("", "root", "11111111", "");
		std::stringstream	ss_line(line);
		std::string			ping;

		script = SCRIPT_PATH "pingcheck.sh";
		ss_line >> n_router.serial_number;
		ss_line >> n_router.ip;
		ping = ScriptExecutor::getOutput::execute(3, script.c_str(), "1", n_router.ip.c_str());
		try {
			if (std::stoi(ping) > 0)
				this->_list_routers.push_back(n_router);
		} catch (std::exception &e) {}
	}
	for (RouterData rou : this->_list_routers)
		std::cerr << "ROUTER-satellit: " << rou.ip << "\n";
}


std::vector<RouterData>     &RouterInfoController::get_routers_info(){
    // Lock    lock(RouterInfoController::_mutex, "RouterInfoController");

    return this->_list_routers;
}

RouterData                  &RouterInfoController::get_self_info(){
    // Lock    lock(RouterInfoController::_mutex, "RouterInfoController");

    return this->_self_info;
}

RouterData                  &RouterInfoController::get_server_info(){
    std::fstream                config_file;
    std::string                 ip;
    // struct sockaddr_in          c_addr;
    // Lock    lock(RouterInfoController::_mutex, "RouterInfoController");

    config_file.open(CONFIG_FILE_PATH);
    getline(config_file, ip);
	std::cerr << "RouterInfoController::get_server_info, CONFIG_FILE_PATH, ip: *" << ip << "*\n";
    // if (!inet_aton(ip.c_str(), (in_addr*)&c_addr)) {
    //     this->_server_info.ip = "";
    //     this->_server_info.login = "";
    //     this->_server_info.pass = "";
    //     config_file.close();
    //     return this->_server_info;
    // }
	/*else*/{
        bool is_find = false;
        for (RouterData router : this->_list_routers) {
			std::cerr << "Router from list: *" << router.ip << "*\n";
            if (router.ip == ip) {
                this->_server_info.ip = ip;
                this->_server_info.login = router.login;
                this->_server_info.pass = router.pass;
                is_find = true;
            }
        }
        if (!is_find) {
            this->_server_info.ip = "";
            this->_server_info.login = "";
            this->_server_info.pass = "";
        }
    }
    config_file.close();
    return this->_server_info;
}



std::string             RouterInfoController::get_info_for_cloud() {
	DeviceInfo              	dev(this->_self_info);
	std::string 				connect_devices_info_str;
	std::string 				script_for_conn_devices = SCRIPT_PATH "station_dump.sh";
	std::vector<std::string>	type802_segments;

	connect_devices_info_str = ScriptExecutor::getOutput::execute(1, script_for_conn_devices.c_str());
	type802_segments = Parser::custom_split(connect_devices_info_str, "Stations on ");
	for (std::string stations : type802_segments) {
		std::stringstream			ss(stations);
		std::string 				type802;
		std::vector<std::string>	stations_segments;
		std::string 				line;
		std::string 				new_str = "";

		getline(ss, line);
		{
			std::stringstream ss_type802(line);

			ss_type802 >> type802;
		}
		while (getline(ss, line)) {
			new_str += line + "\n";
		}
		stations_segments = Parser::custom_split(new_str, "Station");
		for (std::string station_info : stations_segments) {
			std::stringstream 	ss_station(station_info);
			ConnectedDeviceInfo	connectDevice;
			std::string 		param;


			ss_station >> connectDevice._mac;
			connectDevice._type802 = type802;
			while (getline(ss_station, line)) {
				std::vector<std::string>	params_sigments = Parser::custom_split(line, ":");

				std::cerr << params_sigments[0] << "\n----------------------------------\n";
				if (params_sigments.size() != 2)
					continue;
				if (params_sigments[0] == "signal") {
					try {
						connectDevice._signal = std::stoi(params_sigments[1]);
					} catch (std::exception &e) {}
				}
				if (params_sigments[0] == "connected time") {
					connectDevice._timeWork = params_sigments[1];
				}
				if (params_sigments[0] == "inactive time") {
					connectDevice._inactiveTime = params_sigments[1];
				}
			}
			dev._list_connected_devices.push_back(connectDevice);
		}
	}

    return dev.get_string_info();
}


bool 			RouterInfoController::is_sn_from_mesh(std::string serial_number) {
	if (this->_self_info.serial_number == serial_number)
		return true;
	for (RouterData router : this->_list_routers)
		if (router.serial_number == serial_number)
			return true;
	return false;
}



std::mutex      RouterInfoController::_mutex;












//

#include "controllers/RouterInfoController.hpp"
#include "ScriptExecutor.hpp"
#include "controllers/info_tools/DeviceInfo.hpp"
#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "Mutex.hpp"

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
    // RouterData router_kube("192.168.1.1", "root", "11111111", 1231, "KUBE");
    RouterData router_ubuntu_general("192.168.1.134", "user", "11111111", 1231, "e4:95:6e:45:1d:e2");
    // RouterData router_ubuntu_satelit("192.168.1.218", "user", "11111111", 1231, "01201905270000100001");


    // std::string line = ScriptExecutor::getOutput::execute(1, "/tmp/scripts/some.sh");
    // if (line == "ubuntu_general\n") {
        this->_self_info = router_ubuntu_general;
    // }
    // else if (line == "ubuntu_satelit\n") {
    //     this->_self_info = router_ubuntu_satelit;
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


std::vector<RouterData>     RouterInfoController::get_routers_info(){
    // Lock    lock(RouterInfoController::_mutex, "RouterInfoController");

    return this->_list_routers;
}

RouterData                  RouterInfoController::get_self_info(){
    // Lock    lock(RouterInfoController::_mutex, "RouterInfoController");

    return this->_self_info;
}

RouterData                  RouterInfoController::get_server_info(){
    std::fstream                config_file;
    std::string                 ip;
    struct sockaddr_in          c_addr;
    // Lock    lock(RouterInfoController::_mutex, "RouterInfoController");

    config_file.open(CONFIG_FILE_PATH);
    getline(config_file, ip);
    if (!inet_aton(ip.c_str(), (in_addr*)&c_addr)) {
        this->_server_info.ip = "";
        this->_server_info.login = "";
        this->_server_info.pass = "";
        config_file.close();
        return this->_server_info;
    }
    else {
        bool is_find = false;
        for (RouterData router : this->_list_routers) {
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
    Lock    lock(RouterInfoController::_mutex, "RouterInfoController");

    DeviceInfo              test_dev(this->_self_info);
    ConnectedDeviceInfo     connectDevice1(1);
    ConnectedDeviceInfo     connectDevice2(2);
    ConnectedDeviceInfo     connectDevice3(3);

    test_dev._list_connected_devices.push_back(connectDevice1);
    test_dev._list_connected_devices.push_back(connectDevice2);
    test_dev._list_connected_devices.push_back(connectDevice3);
    // return DeviceInfo().get_string_info();
    // std::string line = "Privet :*\n privet :* \n privet :* \n privet :* \n privet :* \nfrom ;)";

    return test_dev.get_string_info();
}

std::mutex      RouterInfoController::_mutex;












//

#include "controllers/RouterInfoController.hpp"
#include "ScriptExecutor.hpp"

RouterInfoController::RouterInfoController() {
}

RouterInfoController::~RouterInfoController() {}

RouterInfoController &RouterInfoController::getInstance() {
    static RouterInfoController info_controller;

    info_controller.refresh();
    return info_controller;
}

void                        RouterInfoController::refresh() {
    // need write scripts for take info from system about routers (DHCP)
    RouterData router_kube("192.168.1.1", "root", "11111111", 1231, "KUBE");
    RouterData router_ubuntu("192.168.1.134", "user", "11111111", 1231, "01201905270000100001");

    std::string line = ScriptExecutor::getOutput::execute(1, "../scripts/some.sh");
    if (line == "ubuntu\n") {
        this->_self_info = router_ubuntu;
    }
    else
        this->_self_info = router_kube;
    this->_list_routers.clear();
    // if (this->_self_info.ip != router_kube.ip)
    //     this->_list_routers.push_back(router_kube);
    // if (this->_self_info.ip != router_ubuntu.ip)
    //     this->_list_routers.push_back(router_ubuntu);
}


std::vector<RouterData>     RouterInfoController::get_routers_info(){
    return this->_list_routers;
}

RouterData                  RouterInfoController::get_self_info(){
    return this->_self_info;
}

RouterData                  RouterInfoController::get_server_info(){
    std::fstream       config_file;
    std::string         ip;
    struct sockaddr_in  c_addr;

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
    // return DeviceInfo().get_string_info();
    std::string line = "Privet :*\n privet :* \n privet :* \n privet :* \n privet :* \nfrom ;)";

    return line;
}














//

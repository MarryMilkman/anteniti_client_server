#ifndef ROUTER_INFO_CONTROLLER
#define ROUTER_INFO_CONTROLLER

#include "lib.h"

class RouterInfoController {
    RouterInfoController();
public:
    RouterInfoController(RouterInfoController const &ref) = delete;
    RouterInfoController operator=(RouterInfoController const &ref) = delete;
    ~RouterInfoController();
    static RouterInfoController &getInstance();

    void                        refresh();

    std::vector<RouterData>     get_routers_info();
    RouterData                  get_self_info();
    RouterData                  get_server_info();

    std::string                 get_info_for_cloud();
private:
    RouterData                  _self_info;
    RouterData                  _server_info;
    std::vector<RouterData>     _list_routers;
};

#endif

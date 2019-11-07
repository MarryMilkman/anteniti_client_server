#ifndef ROUTER_INFO_CONTROLLER
#define ROUTER_INFO_CONTROLLER

#include "lib.h"

class ConnectedDeviceInfo;

class RouterInfoController {
    RouterInfoController();
public:
    RouterInfoController(RouterInfoController const &ref) = delete;
    RouterInfoController operator=(RouterInfoController const &ref) = delete;
    ~RouterInfoController();
    static RouterInfoController &getInstance();

    void                        refresh();
	void 						refresh_satellites_list();

    std::vector<RouterData>				&get_routers_info();
    RouterData							&get_self_info();
    RouterData							&get_server_info();
	struct json_object					*get_router_info_json();

	static std::vector<ConnectedDeviceInfo>	get_list_connected_devices();


	bool 								is_sn_from_mesh(std::string serial_number);
private:
    std::mutex					_mutex;

    RouterData                 	_self_info;
    RouterData                  _server_info;
    std::vector<RouterData>     _list_routers;
};

#endif

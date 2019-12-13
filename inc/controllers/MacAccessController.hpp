#ifndef MAC_ACCESS_CONTROLLER
# define MAC_ACCESS_CONTROLLER

# include "lib.h"
# include "controllers/AccessController.hpp"

class MacAccessController {
	MacAccessController();
public:
    MacAccessController(MacAccessController const &ref) = delete;
    MacAccessController operator=(MacAccessController const &ref) = delete;
    ~MacAccessController();
    static MacAccessController &getInstance();

	void 		add_mac_to_group(std::string mac, std::string group);
	void 		block_mac(std::string mac);
private:
	AccessController	&_access_controller;

	void 		_delete_rule_for_mac(std::string mac);
}

#endif

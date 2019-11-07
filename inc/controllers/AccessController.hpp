#ifndef ACCESS_CONTROLLER
# define ACCESS_CONTROLLER

# include "lib.h"
# include "controllers/StatusController.hpp"
# include "controllers/CloudController.hpp"

enum eAccessLevel {
	al_Limited = 0,
	al_Blocked = 1,
	al_Guest = 2,
	al_General = 3,
	// al_GuestGeneral = 3,
	al_Smart = 4
};

class EventConnect;

class AccessController {
	AccessController();
public:
    AccessController(AccessController const &ref) = delete;
    AccessController operator=(AccessController const &ref) = delete;
    ~AccessController();
    static AccessController &getInstance();

	bool		apply_tmp_map_access_level();
	  // for refresh from some file (if cloud -> download)
	bool		refresh_tmp_map_access_level(std::string path_to_file);
	  // for refresh from list connected devices
	bool 		refresh_tmp_map_access_level(std::vector<EventConnect> list_events);

	std::map<std::string /*mac*/, eAccessLevel>	&get_map_access_level();
	std::mutex 									self_mutex;

private:
	std::map<std::string /*mac*/, eAccessLevel> _map_access_level;
	std::map<std::string /*mac*/, eAccessLevel>	_tmp_map_access_level;

	StatusController 							&_status_controller;
	CloudController 							&_cloud_controller;

	bool 				_init_tmp_map_from(std::string path_to_file);
	void 				_rewrite_access_list();

	void 				_makeLimited(std::string mac);
	void 				_makeBlocked(std::string mac);
	void 				_makeGuest(std::string mac);
	void 				_makeGeneral(std::string mac);
	void 				_makeSmart(std::string mac);

};

#endif

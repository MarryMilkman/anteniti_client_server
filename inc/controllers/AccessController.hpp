#ifndef ACCESS_CONTROLLER
# define ACCESS_CONTROLLER

# include "lib.h"
# include "controllers/StatusController.hpp"
# include "controllers/CloudController.hpp"
# include "Network.hpp"

enum eAccessPattern {
	al_Limited = 'l',
	al_Blocked = 'b',
	al_Guest = 'g',
	al_Main = 'm',
	// al_GuestGeneral = 3,
	al_Smart = 's'
};

typedef struct		s_accessLevel {
	eAccessPattern	pattern;
	char 			group;
	std::string 	name;
	std::string 	iface;
	s_accessLevel() {
		this->pattern = al_Limited;
		this->group = 0;
		this->name = "";
		this->iface = "wlan-1";
	}
	~s_accessLevel() {}
	s_accessLevel	&operator=(s_accessLevel const & ref) {
		this->pattern = ref.pattern;
		this->group = ref.group;
		this->name = ref.name;
		this->iface = ref.iface;
		return *this;
	}
}					t_accessLevel;

class EventConnect;

class AccessController {
	AccessController();
public:
    AccessController(AccessController const &ref) = delete;
    AccessController operator=(AccessController const &ref) = delete;
    ~AccessController();
    static AccessController &getInstance();

	bool 		apply_access_level_for_mac(std::string mac, std::string iface);
	bool 		apply_map_access_level();
	bool		apply_tmp_map_access_level();
	  // for refresh from some file (if cloud -> download)
	bool		refresh_tmp_map_access_level(std::string path_to_file);
	  // for refresh from list connected devices
	bool 		refresh_tmp_map_access_level(std::vector<EventConnect> list_events);

	std::map<std::string /*mac*/, t_accessLevel>	&get_map_access_level();
	std::map<std::string /*name*/, Network>			&get_map_networks();
	std::mutex 										self_mutex;

private:
	std::map<std::string /*mac*/, t_accessLevel> 	_map_access_level;
	std::map<std::string /*mac*/, t_accessLevel>	_tmp_map_access_level;
	std::map<std::string /*name*/, Network>			_map_networks;

	StatusController 							&_status_controller;
	CloudController 							&_cloud_controller;

	bool 				_init_tmp_map_from(std::string path_to_file);
	void 				_rewrite_access_list();

	void 				_choose_pattern_and_execute(std::string mac, t_accessLevel access_level);

	void 				_makeLimited(std::string mac, t_accessLevel access_level);
	void 				_makeBlocked(std::string mac, t_accessLevel access_level);
	void 				_makeGuest(std::string mac, t_accessLevel access_level);
	void 				_makeMain(std::string mac, t_accessLevel access_level);
	void 				_makeSmart(std::string mac, t_accessLevel access_level);

};

#endif

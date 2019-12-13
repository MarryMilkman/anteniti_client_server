#ifndef ACCESS_CONTROLLER
# define ACCESS_CONTROLLER

# include "lib.h"
# include "controllers/StatusController.hpp"
# include "controllers/CloudController.hpp"
# include "controllers/RouterInfoController.hpp"
# include "Network.hpp"

enum eAccessPattern {
	al_Limited = 'z',
	al_Blocked = 'b',
	al_Guest = 'G',
	al_Main = 'L',
	// al_GuestGeneral = 3,
	al_Smart = 'S'
};

typedef struct		s_accessLevel {
	// eAccessPattern				pattern;
	// std::vector<std::string>	list_group;
	// char 						group;
	std::string 				name_group;
	std::string 				old_name_group;
	std::string 				iface;
	bool 						need_deauth;

	s_accessLevel();
	~s_accessLevel();
	s_accessLevel	&operator=(s_accessLevel const & ref);
}					t_accessLevel;

class EventConnect;

class AccessController {
	AccessController();
public:
    AccessController(AccessController const &ref) = delete;
    AccessController operator=(AccessController const &ref) = delete;
    ~AccessController();
    static AccessController &getInstance();

	// bool 		apply_access_level_for_mac(std::string mac, std::string ip, bool is_conn);
	// bool 		apply_map_access_level();
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
	RouterInfoController						&_info_controller;

	bool 				_init_tmp_map_from(std::string path_to_file);
	void 				_rewrite_access_list();

	std::string			_form_scripts_access(
									std::string mac,
									std::string name_group,
									char flag);

	bool				_is_mac_in_group(std::string mac, std::string group);
	std::string 		_get_flag_group(std::string name_group);

	void 				_add_mac_to_group(std::string mac,
										std::string group,
										std::map<std::string /*mac*/, t_accessLevel> &map_access_level);
	void 				_block_mac(std::string mac);
	void 				_delete_rule_for_mac(std::string mac);

	// void 				_makeLimited(std::string mac, t_accessLevel access_level);
	// void 				_makeBlocked(std::string mac, t_accessLevel access_level);
	// void 				_makeGuest(std::string mac, t_accessLevel access_level);
	// void 				_makeMain(std::string mac, t_accessLevel access_level);
	// void 				_makeSmart(std::string mac, t_accessLevel access_level);

};

#endif

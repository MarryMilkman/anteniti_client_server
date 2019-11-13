#ifndef ACCESS_CONTROLLER
# define ACCESS_CONTROLLER

# include "lib.h"
# include "controllers/StatusController.hpp"
# include "controllers/CloudController.hpp"

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
	s_accessLevel() {
		this->pattern = al_Limited;
		this->group = 0;
		this->name = "";
	}
	~s_accessLevel() {}
	s_accessLevel	&operator=(s_accessLevel const & ref) {
		this->pattern = ref.pattern;
		this->group = ref.group;
		this->name = ref.name;
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

	bool 		apply_acces_level_for_mac(std::string mac);

	bool		apply_tmp_map_access_level();
	  // for refresh from some file (if cloud -> download)
	bool		refresh_tmp_map_access_level(std::string path_to_file);
	  // for refresh from list connected devices
	bool 		refresh_tmp_map_access_level(std::vector<EventConnect> list_events);

	std::map<std::string /*mac*/, t_accessLevel>	&get_map_access_level();
	std::mutex 										self_mutex;

private:
	std::map<std::string /*mac*/, t_accessLevel> _map_access_level;
	std::map<std::string /*mac*/, t_accessLevel>	_tmp_map_access_level;

	StatusController 							&_status_controller;
	CloudController 							&_cloud_controller;

	bool 				_init_tmp_map_from(std::string path_to_file);
	void 				_rewrite_access_list();

	void 				_makeLimited(std::string mac, int group);
	void 				_makeBlocked(std::string mac, int group);
	void 				_makeGuest(std::string mac, int group);
	void 				_makeGeneral(std::string mac, int group);
	void 				_makeSmart(std::string mac, int group);

};

#endif

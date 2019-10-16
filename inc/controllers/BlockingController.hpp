#ifndef BLOCKING_CONTROLLER_HPP
# define BLOCKING_CONTROLLER_HPP

# include "lib.h"

#define DIR_BLOCKLIST "/tmp/blocking/"
#define PATH_BLOCKLIST "/tmp/blocking/blocking.txt"

enum eAccessLevel {
	al_Limited = 0,
	al_Blocked = 1,
	al_Guest = 2,
	al_General = 3,
	// al_GuestGeneral = 3,
	al_Smart = 4
};

// 0 || wlan0 - general 2.4g
// 1 - mesh
// 2 || wlan0-1 - 192.168.1.3 - back
// 3 || wlan1-1 - general 5g
// 4 || wlan0-2 - sump 2.4g
// 5 || wlan1-2 - sump 5g
// 6 - anteniti
// 7 || wlan0-3 - guest 2.4
// 8 || wlan1-3 - guest 5g

enum eNumWireless {
	nw_General = 0, // 3
	nw_General5 = 3,
	nw_Sump = 4, // 5
	nw_Sump5 = 5,
	nw_Guest = 7, // 8
	nw_Guest5 = 8,
	nw_Smurt = -1
};


struct BlockDevice {
	BlockDevice();
	~BlockDevice();
	BlockDevice(BlockDevice const &ref);
	BlockDevice 	&operator=(BlockDevice const &ref);

	std::string 	mac;
	eAccessLevel	access_level;
};


class BlockingController {
	BlockingController();
public:
	BlockingController(BlockingController const &ref) = delete;
	BlockingController &operator=(BlockingController const &ref) = delete;
	~BlockingController();
	static BlockingController    &getInstance();

	bool 			download_list();
	bool 			apply();

private:
	std::vector<BlockDevice>	_tmp_block_list;

	void 		_get_block_list_from_file();
	bool 		_is_mac_from(std::string mac, eNumWireless num_wireless);

	void 		_makeLimited(std::string mac);
	void 		_makeBlocked(std::string mac);
	void 		_makeGuest(std::string mac);
	void 		_makeGeneral(std::string mac);
	void 		_makeSmart(std::string mac);
	void 		_wifi_relaod();

};


#endif

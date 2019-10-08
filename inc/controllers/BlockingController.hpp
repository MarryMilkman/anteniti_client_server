#ifndef BLOCKING_CONTROLLER_HPP
# define BLOCKING_CONTROLLER_HPP

# include "lib.h"

#define DIR_BLOCKLIST "/tmp/blocking/"
#define PATH_BLOCKLIST "/tmp/blocking/blocking.txt"

enum eAccessLevel {
	al_Blocked = 0,
	al_Guest = 1,
	al_General = 2,
	al_GuestGeneral = 3,
	al_Smart = 4
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

	bool 		_is_mac_from(std::string mac, std::string name_network);


};


#endif

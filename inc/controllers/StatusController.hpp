#ifndef STATUS_CONTROLLER
# define STATUS_CONTROLLER

#include "lib.h"

enum eWorkMod
{
    wm_unknown = 0,
    wm_server,
    wm_client
};

class StatusController
{
    StatusController();
public:
    StatusController(StatusController const &ref) = delete;
    StatusController    &operator=(StatusController const &ref) = delete;
    ~StatusController();
    static StatusController &getInstance();

    static bool             isWAN();

    bool                    server_availabilit;

	void 					wifi_reload();
	void 					mac_list_reload();

    void                    choiceModWork();
    eWorkMod                getWorkMod();
	time_t 					get_time_reload();
private:
    // static std::mutex      _mutex;
	time_t			_time_reload;

    eWorkMod        _mod;

    void            _scan();
    void            _startServer();
    void            _startClient();
    void            _fresh();
};

#endif

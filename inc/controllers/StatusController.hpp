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

    void                    choiceModWork();
    eWorkMod                getWorkMod();
private:
    eWorkMod        _mod;

    void            _scan();
    void            _startServer();
    void            _startClient();
    void            _fresh();
};

#endif

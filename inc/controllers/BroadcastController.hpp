#ifndef BROADCAST_CONTROLLER_HPP
# define BROADCAST_CONTROLLER_HPP

# include "lib.h"

#define BCC_ADDR "192.168.1.255"
#define BCC_PORT 9390

class BroadcastController
{
    BroadcastController();
public:
    BroadcastController(BroadcastController const & ref) = delete;
    BroadcastController &operator=(BroadcastController const & ref) = delete;
    ~BroadcastController();
    static BroadcastController &getInstance();

    void                send(std::string message, int nbr_sending);
    int                 receive(int timeout);

    std::string         get_message() const;

private:
    std::string         _message;
    int                 _sockfd;
    struct sockaddr_in  _sendaddr;
    struct sockaddr_in  _recvaddr;

    void                _dataInit_send();
    void                _dataInit_recave();

    bool                _is_cast_from_server();

    void                _initGeneralData();
};

#endif

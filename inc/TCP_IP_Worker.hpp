#ifndef TCP_IP_WORKER
# define TCP_IP_WORKER

# include "lib.h"


// #define LISTEN_PORT 9742

class TCP_IP_Worker {
public:
    TCP_IP_Worker();
    ~TCP_IP_Worker();

    int         acceptMessage(int port, int timeout_s);
    std::string get_connected_ip() const;
    std::string get_message() const;
    int         tcp_disconnect();

    int     tcp_connect(std::string ip, int port, int timeout_s);
    int     tcp_send(std::string message);

private:
        //general
    int                 _sockfd;
    struct sockaddr_in  _addres;
        //server part
    struct sockaddr_in  _accepted_addres;
    int                 _accepted_sockfd;
        //client part
    struct sockaddr_in  _addr_for_connect;

    std::string         _message;

    int             _init_for_accept(int port);
    int             _init_for_connect(std::string ip, int port);
    std::string	    _read_from_sock(int sock);

};

#endif

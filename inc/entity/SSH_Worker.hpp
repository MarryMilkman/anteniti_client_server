#ifndef SSH_WORKER_HPP
# define SSH_WORKER_HPP

# include "lib.h"

// # define SCP_PATH "/tmp/variable_setting.txt"

class SSH_Worker {
public:
    SSH_Worker();
    SSH_Worker(std::string login, std::string ip, std::string pass);
    // SSH_Worker(SSH_Worker const & ref);
    // SSH_Worker &operator=(SSH_Worker const & ref);
    ~SSH_Worker();

    std::string     login;
    std::string     ip;
    std::string     pass;

    int         scp(std::string file_path, std::string scp_path);

    // std::string execute(std::string exec_mess);
private:
    int                 _sockfd;
    sockaddr_in         _addres;

    LIBSSH2_SESSION     *_session = NULL;
    LIBSSH2_CHANNEL     *_channel;

    std::string         _scp_file;
    std::string         _scp_path;

    int        _connect_sock();
    int        _init_and_auth();

    void        _print_error(std::string title);
};

#endif

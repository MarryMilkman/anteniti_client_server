#include "controllers/BroadcastController.hpp"
#include "controllers/RouterInfoController.hpp"
#include "Encryptor.hpp"
#include "Mutex.hpp"


BroadcastController::BroadcastController() {

}

BroadcastController::~BroadcastController() {
    if (this->_sockfd > 2)
        close(this->_sockfd);
}

BroadcastController &BroadcastController::getInstance() {
    Lock    lock(BroadcastController::_mutex, "BroadcastController");
    static BroadcastController  bc_controller;

    return bc_controller;
}

// MARK : send - send broadcast whith chuse message for router-client
void    BroadcastController::send(std::string message, int nbr_iterat) {
    int         num_b = 0;
    int         i = -1;
    char        *buff;
    std::string encrypt_mess;
    Encryptor   e;

    buff = e.encrypt(message);
    encrypt_mess = buff;
    free(buff);
    Lock    lock(BroadcastController::_mutex, "BroadcastController");
    this->_dataInit_send();
    std::cerr << "Broadcast send: " << message << "\n";
    while(++i < nbr_iterat)
    {
        usleep(1500);
        num_b = sendto(this->_sockfd, encrypt_mess.c_str(), encrypt_mess.size(), 0, (struct sockaddr*)&this->_recvaddr, sizeof(this->_recvaddr));
        if (num_b == -1) {
            perror("Broadcast send (sendto):");
            if (this->_sockfd > 2)
                close(this->_sockfd);
            throw std::exception();
        }
    }
    if (this->_sockfd > 2)
        close(this->_sockfd);
}

    // init data for send broadcast
void    BroadcastController::_dataInit_send() {
    int answer_bind;

    this->_initGeneralData();
    this->_recvaddr.sin_family = AF_INET;
    this->_recvaddr.sin_port = htons(BCC_PORT);
    this->_recvaddr.sin_addr.s_addr = inet_addr(BCC_ADDR);
    memset(this->_recvaddr.sin_zero, '\0', sizeof(this->_recvaddr.sin_zero));
    answer_bind = bind(this->_sockfd, (struct sockaddr*) &this->_sendaddr, sizeof(this->_sendaddr));
    if (answer_bind < 0) {
        perror("Broadcast bind send init:");
        throw std::exception();
    }
}



// MARK : receive - wait broadcast with message from router-server
int     BroadcastController::receive(int timeout_s) {
    int     sendaddr_len;
    int     num_b;
    char    buff[1024];
    fd_set          readfds;
    struct timeval  timeout;

    timeout.tv_sec = timeout_s;
    timeout.tv_usec = 0;
    Lock    lock(BroadcastController::_mutex, "BroadcastController");
    this->_dataInit_recave();
    sendaddr_len = sizeof(this->_sendaddr);
    FD_ZERO(&readfds);
    FD_SET(this->_sockfd , &readfds);
    int activity = select(this->_sockfd + 1, &readfds, 0, 0, &timeout);
    if (activity <= 0) {
        std::cerr << "Broadcast recave, timeout\n";
        if (this->_sockfd > 2)
            close(this->_sockfd);
        return -1;
    }
    if ((num_b = recvfrom(this->_sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&this->_sendaddr, (socklen_t *)&sendaddr_len)) == -1)
    {
        perror("recvfrom");
        return -1;
    }
    if (!this->_is_cast_from_server()) {
        this->_message = "Uncnown sender\n";
        return -1;
    }
    buff[num_b] = 0;
    this->_message = Encryptor::decrypt(buff);
    if (this->_sockfd > 2)
        close(this->_sockfd);
    return 0;
}
    // init data for receive broadcast
void    BroadcastController::_dataInit_recave() {
    int answer_bind;

    this->_initGeneralData();
    this->_recvaddr.sin_family = AF_INET;
    this->_recvaddr.sin_port = htons(BCC_PORT);
    this->_recvaddr.sin_addr.s_addr = INADDR_ANY;
    memset(this->_recvaddr.sin_zero, '\0', sizeof(this->_recvaddr.sin_zero));
    answer_bind = bind(this->_sockfd, (struct sockaddr*) &this->_recvaddr, sizeof(this->_recvaddr));
    if (answer_bind < 0) {
        perror("Broadcast bind recave init:");
        throw std::exception();
    }
}



    //init general seting
void    BroadcastController::_initGeneralData() {
    int     broadcast = 1;

    this->_message = "";
    this->_sockfd = socket(PF_INET,SOCK_DGRAM,0);
    if (this->_sockfd <= 0)
	{
		perror("sockfd");
		throw std::exception();
	}
    setsockopt(this->_sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEPORT, &broadcast, sizeof(broadcast));
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast));
    this->_sendaddr.sin_family = AF_INET;
	this->_sendaddr.sin_port = htons(BCC_PORT);
	this->_sendaddr.sin_addr.s_addr = INADDR_ANY;
    memset(this->_sendaddr.sin_zero, '\0', sizeof(this->_sendaddr.sin_zero));
}


bool            BroadcastController::_is_cast_from_server() {
    std::string ip_server = RouterInfoController::getInstance().get_server_info().ip;
    std::string ip_broadcast = inet_ntoa(this->_sendaddr.sin_addr);

    std::cerr << ip_server << " : " << ip_broadcast << "\n";
    if (ip_server != ip_broadcast)
        return false;
    return true;
}

    //
std::string     BroadcastController::get_message() const {
    Lock    lock(BroadcastController::_mutex, "BroadcastController");

    return this->_message;
}

std::mutex   BroadcastController::_mutex;

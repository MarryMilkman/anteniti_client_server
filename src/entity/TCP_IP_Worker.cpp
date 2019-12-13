#include "TCP_IP_Worker.hpp"
#include "Encryptor.hpp"

TCP_IP_Worker::TCP_IP_Worker() {
    this->_sockfd = 0;
    this->_accepted_sockfd = 0;
}

TCP_IP_Worker::~TCP_IP_Worker() {
    if (this->_sockfd)
        close(this->_sockfd);
    if (this->_accepted_sockfd)
        close(this->_accepted_sockfd);
    this->_sockfd = 0;
    this->_accepted_sockfd = 0;
}

// MARK : servers metods
int         TCP_IP_Worker::acceptMessage(int port, int timeout_s) {
    socklen_t       acc_add_len = sizeof(this->_accepted_addres);
    fd_set          readfds;
    struct timeval  timeout;

    timeout.tv_sec = timeout_s;
    timeout.tv_usec = 0;
    memset(this->_accepted_addres.sin_zero, 0, sizeof(this->_accepted_addres.sin_zero));
    if (!this->_sockfd)
        this->_init_for_accept(port);
    if (listen(this->_sockfd, 5) < 0) {
        std::cerr << "listen fail\n";
        this->_init_for_accept(port);
        return -1;
    }
    FD_ZERO(&readfds);
    FD_SET(this->_sockfd , &readfds);
    int activity = select(this->_sockfd + 1, &readfds, 0, 0, &timeout);
    // std::cerr << activity << " - connected sockets\n";
    if (activity > 0)
        this->_accepted_sockfd = accept(this->_sockfd, (struct sockaddr *)&this->_accepted_addres, &acc_add_len);
    else {
        // std::cerr << "Listen timeout\n";
        return -1;
    }
    if (this->_accepted_sockfd <= 0) {
        std::cerr << "accept fail\n";
        return -1;
    }
    this->_message = Encryptor::decrypt(this->_read_from_sock(this->_accepted_sockfd));
    return 0;
}





int         TCP_IP_Worker::tcp_disconnect() {
    if (this->_sockfd)
        close(this->_sockfd);
    this->_sockfd = 0;
    return 0;
}

std::string TCP_IP_Worker::get_connected_ip() const {
    return inet_ntoa(this->_accepted_addres.sin_addr);
}




// MARK : client metods
int         TCP_IP_Worker::tcp_connect(std::string ip, int port, int timeout_s) {
    this->_init_for_connect(ip, port);
    fd_set          readfds;
    struct timeval  timeout;

    timeout.tv_sec = timeout_s;
    timeout.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(this->_sockfd , &readfds);
    int activity = select(this->_sockfd + 1, &readfds, 0, 0, &timeout);
    if (activity <= 0) {
        std::cerr << "connect timeout\n";
        return -1;
    }
    if (connect(this->_sockfd, (struct sockaddr *)&this->_addr_for_connect, sizeof(this->_addr_for_connect)) < 0)
        return -1;
    return 0;
}

int         TCP_IP_Worker::tcp_send(std::string message) {
	Encryptor   e;
    char        *buff;
    int         size = 0;

    buff = e.encrypt(message);
    while (buff[size]) size++;
    if (send(this->_sockfd, buff, size, 0) < 0) {
        std::cerr << "send fail\n";
        if (this->_sockfd)
            close(this->_sockfd);
        this->_sockfd = 0;
        return -1;
    }
    free(buff);
    shutdown(this->_sockfd, 1);
    return 0;
}


// MARK : general metods
std::string TCP_IP_Worker::get_message() const {
    return this->_message;
}



// MARK : init

    // accept
int         TCP_IP_Worker::_init_for_accept(int port) {
    int     opt = 1;

    if (this->_sockfd > 0) {
        close(this->_sockfd);
        this->_sockfd = 0;
    }
    if (this->_accepted_sockfd > 0) {
        close(this->_accepted_sockfd);
        this->_accepted_sockfd = 0;
    }
    if (!(this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)))
        return -1;
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    this->_addres.sin_family = AF_INET;
    this->_addres.sin_port = htons(port);
    this->_addres.sin_addr.s_addr = INADDR_ANY;
    memset(this->_addres.sin_zero, '\0', sizeof(this->_addres.sin_zero));
    int answer = bind(this->_sockfd, (struct sockaddr *)&this->_addres, sizeof(this->_addres));
	if (answer < 0) {
        if (this->_sockfd)
            close(this->_sockfd);
        this->_sockfd = 0;
		perror("bind error");
		return -1;
	}
    return 0;
}

    // connect
int         TCP_IP_Worker::_init_for_connect(std::string ip, int port) {
    int     opt = 1;

    if (this->_sockfd) {
        close(this->_sockfd);
        this->_sockfd = 0;
    }
    if (!(this->_sockfd = socket(AF_INET, SOCK_STREAM, 0)))
		return -1;
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
	this->_addr_for_connect.sin_family = AF_INET;
	this->_addr_for_connect.sin_port = htons(port);
    this->_addr_for_connect.sin_addr.s_addr = inet_addr(ip.c_str());
    memset(this->_addr_for_connect.sin_zero, '\0', sizeof(this->_addr_for_connect.sin_zero));
    return 0;
}


std::string	TCP_IP_Worker::_read_from_sock(int sock) {
	char	    buffer[64];
    std::string message;
    int         byts;

	while ((byts = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[byts] = 0;;
        message += buffer;
        buffer[0] = 0;
    }
    shutdown(sock, 0);
	return message;
}

#include "SSH_Worker.hpp"

SSH_Worker::SSH_Worker() {
	this->_session = 0;
	this->_channel = 0;
	this->_sockfd = 0;
}

SSH_Worker::SSH_Worker(std::string login, std::string ip, std::string pass) {
	this->_session = 0;
	this->_channel = 0;
	this->_sockfd = 0;
    this->login = login;
    this->ip = ip;
    this->pass = pass;
}

SSH_Worker::~SSH_Worker() {
    if (this->_sockfd)
        close(this->_sockfd);
    if (this->_session){
        libssh2_session_disconnect(this->_session, "Normal Shutdown");
        libssh2_session_free(this->_session);
    }
	if (this->_channel) {
		libssh2_channel_wait_closed(this->_channel);
	    libssh2_channel_free(this->_channel);
	}
	std::cerr << "~SSH_Worker end\n";
}

int         SSH_Worker::scp(std::string file_path, std::string scp_path) {
    FILE            *send_file;
    struct stat     fileinfo;
    char            mem[1024];

    if (this->_connect_sock())
        return -1;
    if (this->_init_and_auth())
        return -1;
    send_file = fopen(file_path.c_str(), "rb");
    if (!send_file) {
        std::cerr << "Scp error: file not found - " << file_path << "\n";
        return -1;
    }
    stat(file_path.c_str(), &fileinfo);
    this->_channel = libssh2_scp_send(this->_session, scp_path.c_str(), fileinfo.st_mode & 0777, (unsigned long)fileinfo.st_size);
    if(!this->_channel) {
        this->_print_error("libssh2_scp_send");
        if (send_file)
            fclose(send_file);
        return -1;
    }

    char    *ptr = 0;
    int     nread = 0;
    int     rc = 0;

    while (1) {
        nread = fread(mem, 1, sizeof(mem), send_file);
        mem[nread] = 0;
        if(nread <= 0)
            break;
        ptr = mem;
        while (nread) {
            rc = libssh2_channel_write(this->_channel, ptr, nread);
            if(rc < 0) {
                this->_print_error("libssh2_channel_write");
                break;
            }
            else {
                ptr += rc;
                nread -= rc;
            }
        }
    }
    libssh2_channel_send_eof(this->_channel);
    libssh2_channel_wait_eof(this->_channel);
    libssh2_channel_wait_closed(this->_channel);
    libssh2_channel_free(this->_channel);
    this->_channel = 0;
    if (send_file)
        fclose(send_file);
    return 0;
}



int        SSH_Worker::_connect_sock() {
    int		answer;
	int 	z = 1;

    this->_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    this->_addres.sin_family = AF_INET;
    this->_addres.sin_port = htons(22);
    this->_addres.sin_addr.s_addr = inet_addr(this->ip.c_str());
	setsockopt(this->_sockfd, IPPROTO_TCP, TCP_SYNCNT, &z, sizeof(z));
	// std::cerr << "WAit?\n";
    answer = connect(this->_sockfd, (struct sockaddr*)(&this->_addres), sizeof(this->_addres));
    if (answer) {
		std::string mess = "SSH: connect to " + (this->ip) + " failed:";
        perror(mess.c_str());
        return -1;
    }
    return 0;
}

int        SSH_Worker::_init_and_auth() {
    // std::string     fingerprint;
    std::string     password = this->pass;

    if (!(this->_session = libssh2_session_init())) {
        this->_print_error("libssh2_session_init");
        return -1;
    }
    libssh2_session_set_blocking(this->_session, 1);
    if (libssh2_session_handshake(this->_session, this->_sockfd)) {
        this->_print_error("libssh2_session_handshake");
        return -1;
    }
    // fingerprint = libssh2_hostkey_hash(this->_session, LIBSSH2_HOSTKEY_HASH_SHA1);
    if(libssh2_userauth_password(this->_session, this->login.c_str(), password.c_str())) {
        this->_print_error("libssh2_userauth_password");
        return -1;
    }
    return 0;
// #define HOME "/home/user/"
//     if(libssh2_userauth_publickey_fromfile(this->_session, this->login.c_str(),
//
//                                            HOME ".ssh/test_key.pub",
//                                            HOME ".ssh/test_key",
//                                            password.c_str())) {
//         fprintf(stderr, "\tAuthentication by public key failed\n");
//     }

}

void        SSH_Worker::_print_error(std::string title) {
    char *errmsg;
    int errlen;
	std::cerr << "ch?\n";
    int err = libssh2_session_last_error(this->_session, &errmsg, &errlen, 0);

    if (err)
        std::cerr << title << ": " << errmsg << "\n";
}

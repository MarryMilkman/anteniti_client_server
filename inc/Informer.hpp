#ifndef INFORMER_HPP
# define INFORMER_HPP

# include "lib.h"

// # define SCP_PATH "/tmp/variable_setting.txt"

class Informer {
public:
    Informer();
    // Informer(Informer const & ref);
    // Informer &operator=(Informer const & ref);
    ~Informer();

	void operator()();

private:
	std::string			_message;
	int 				_sockfd;
	struct sockaddr_in	_receive_addr;
	struct sockaddr_in	_bc_addr;

	void 				_init();

	bool				_listen_question(int timeout_s);
	void 				_send_answer(std::string ip, int port, std::string message);

	std::string 		_get_bc_ip();

	std::string			_form_answer_leases_by_mac(std::string mac);


};

#endif

#include "Informer.hpp"
#include "Encryptor.hpp"
#include "TCP_IP_Worker.hpp"
#include "ScriptExecutor.hpp"
#include "controllers/RouterInfoController.hpp"
#include "Parser.hpp"

Informer::Informer() {
	this->_sockfd = 0;
	this->_init();
}

Informer::~Informer() {
	if (this->_sockfd > 2)
		close(this->_sockfd);
}

void 		Informer::_init() {
	// init _bc_addr
	int     broadcast = 1;

	if (this->_sockfd > 2)
		close(this->_sockfd);
    this->_sockfd = socket(PF_INET,SOCK_DGRAM,0);
    if (this->_sockfd <= 0)
	{
		this->_sockfd = 0;
		perror("sockfd");
		return;
	}
    setsockopt(this->_sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEPORT, &broadcast, sizeof(broadcast));
    setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, &broadcast, sizeof(broadcast));
    this->_bc_addr.sin_family = AF_INET;
	this->_bc_addr.sin_port = htons(Constant::Inform::bc_port);
	this->_bc_addr.sin_addr.s_addr = INADDR_ANY;
    memset(this->_bc_addr.sin_zero, '\0', sizeof(this->_bc_addr.sin_zero));
	// init _receive_addr
	int answer_bind;

	this->_receive_addr.sin_family = AF_INET;
	this->_receive_addr.sin_port = htons(Constant::Inform::bc_port);
	this->_receive_addr.sin_addr.s_addr = INADDR_ANY;
	memset(this->_receive_addr.sin_zero, '\0', sizeof(this->_receive_addr.sin_zero));
	answer_bind = bind(this->_sockfd, (struct sockaddr*) &this->_receive_addr, sizeof(this->_receive_addr));
	if (answer_bind < 0) {
		perror("Broadcast bind recave init");
		if (this->_sockfd > 2) {
			close(this->_sockfd);
			this->_sockfd = 0;
		}
	}
}






void 		Informer::operator()() {
	while (1) {
		if (!this->_listen_question(3))
			continue;
		std::vector<std::string>	question_param = Parser::custom_split(this->_message, Constant::Inform::delim);
		std::string 				answer_message;
		int 						size = question_param.size();

		if (size > 2)
			continue;
		if (question_param[0] == Constant::Inform::ask_lease_by_mac && size == 2) {
			answer_message = this->_form_answer_leases_by_mac(question_param[1]);
		}
		else
			continue;
		if (!answer_message.empty())
			this->_send_answer(this->_get_bc_ip(), Constant::Inform::answer_port, answer_message);
		std::cerr << "Informer: send answer: OK\n";
	}
}

bool 		Informer::_listen_question(int timeout_s) {
	int     sendaddr_len;
	int     num_b;
	char    buff[512];
	fd_set          readfds;
	struct timeval  timeout;

	if (!this->_sockfd) {
		this->_init();
		return false;
	}
	timeout.tv_sec = timeout_s;
	timeout.tv_usec = 0;
	sendaddr_len = sizeof(this->_bc_addr);
	FD_ZERO(&readfds);
	FD_SET(this->_sockfd , &readfds);
	int activity = select(this->_sockfd + 1, &readfds, 0, 0, &timeout);
	if (activity <= 0) {
		// std::cerr << "Broadcast recave, timeout\n";
		return false;
	}
	if ((num_b = recvfrom(this->_sockfd, buff, sizeof(buff), 0, (struct sockaddr *)&this->_bc_addr, (socklen_t *)&sendaddr_len)) == -1)
	{
		perror("recvfrom");
		return false;
	}
	buff[num_b] = 0;
	this->_message = Encryptor::decrypt(buff);
	std::cerr << "Informer: new question: " << this->_message << "\n";
	return true;
}


void 		Informer::_send_answer(std::string ip, int port, std::string message) {
	TCP_IP_Worker 			tcp_ip;
	int 					i = 0;

	if (ip.empty()){
		std::cerr << "ERROR: no destination for send\n";
		return ;
	}
	while (tcp_ip.tcp_connect(ip, port, 1)) {
		if (i > 100)
			break ;
		usleep(200);
		i++;
	}
	if (i > 100) {
		std::cerr << "(ANSWER NOT SENDED): " << message << "\n";
		perror("reason: ");
		return ;
	}
	std::cerr << "success connect, try: " << i << "\n";
	tcp_ip.tcp_send(message);
	return ;
}

std::string Informer::_get_bc_ip() {
	return (inet_ntoa(this->_bc_addr.sin_addr));
}


// MARK : form answer

std::string		Informer::_form_answer_leases_by_mac(std::string mac) {
	std::map<std::string, std::string>	map_info_dev = RouterInfoController::get_dev_info_by_mac(mac);

	if (map_info_dev["nick"].empty())
		return std::string();
	return (mac + " " + map_info_dev["ip"] + " " + map_info_dev["nick"]);
	// std::string 								script_check_leases;
	//
	// script_check_leases = "cat " + Constant::Files::leases_info + " | grep " + mac;
	// return ScriptExecutor::getOutput::execute(1, script_check_leases.c_str());
}

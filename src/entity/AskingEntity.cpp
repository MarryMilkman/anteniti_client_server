#include "AskingEntity.hpp"
#include "Encryptor.hpp"
#include "TCP_IP_Worker.hpp"


AskingEntity::AskingEntity() {
	this->_init();
}

AskingEntity::~AskingEntity() {
	if (this->_sockfd > 2)
		close(this->_sockfd);
}

void 		AskingEntity::_init() {
	// init _bc_addr
	int     broadcast = 1;

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
	// init _send_addr
	int answer_bind;

	this->_send_addr.sin_family = AF_INET;
    this->_send_addr.sin_port = htons(Constant::Inform::bc_port);
    this->_send_addr.sin_addr.s_addr = inet_addr(Constant::Inform::bc_addr.c_str());
	memset(this->_send_addr.sin_zero, '\0', sizeof(this->_send_addr.sin_zero));
	answer_bind = bind(this->_sockfd, (struct sockaddr*) &this->_send_addr, sizeof(this->_send_addr));
	if (answer_bind < 0) {
		perror("Broadcast bind recave init");
		if (this->_sockfd > 2) {
			close(this->_sockfd);
			this->_sockfd = 0;
		}
	}
}

	// return true if there is at least one answer
bool 			AskingEntity::ask_everyone(std::string type_question, std::string optional, int timeout) {
	std::string 	question = type_question + Constant::Inform::delim + optional;

	this->_send_question(question);
	return this->_listen_answer(timeout);
}

bool 			AskingEntity::_send_question(std::string question) {
	int         num_b = 0;
	int         i = -1;
	char        *buff;
	std::string encrypt_mess;
	Encryptor   e;

	buff = e.encrypt(question);
	encrypt_mess = buff;
	free(buff);
	this->_init();
	std::cerr << "AskingEntity broadcast send: " << question << "\n";
	num_b = sendto(this->_sockfd, encrypt_mess.c_str(), encrypt_mess.size(), 0, (struct sockaddr*)&this->_send_addr, sizeof(this->_send_addr));
	if (num_b == -1) {
		perror("Broadcast send (sendto):");
		if (this->_sockfd > 2)
			close(this->_sockfd);
		return false;
	}
	if (this->_sockfd > 2)
		close(this->_sockfd);
	return true;
}

bool			AskingEntity::_listen_answer(int timeout) {
	TCP_IP_Worker 			listener;
	int 					count = 0;

	this->_map_answer.clear();
	while (count < 7 && listener.acceptMessage(Constant::Inform::answer_port, timeout) == 0) {
		std::string message = listener.get_message();
		std::string ip = listener.get_connected_ip();

		std::cerr << ip << " connect ip, mess: " << message << "\n";
		if (this->_map_answer.count(ip))
			continue;
		this->_map_answer[ip] = message;
	}
	if (this->_map_answer.empty())
		return false;
	return true;
}

std::map<std::string /*ip*/, std::string /*answer*/>	AskingEntity::get_map_answer() const {
	return this->_map_answer;
}






//

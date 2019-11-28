#ifndef ASKING_ENTITY_HPP
# define ASKING_ENTITY_HPP

# include "lib.h"

// # define SCP_PATH "/tmp/variable_setting.txt"

class AskingEntity {
public:
    AskingEntity();
    // AskingEntity(AskingEntity const & ref);
    // AskingEntity &operator=(AskingEntity const & ref);
    ~AskingEntity();

	// bool 			ask_one(std::string question, int timeout, str::string ip);
	bool 													ask_everyone(std::string type_question, std::string optional, int timeout);
	std::map<std::string /*ip*/, std::string /*answer*/>	get_map_answer() const;

private:
	std::string												_message;
	int 													_sockfd;
	struct sockaddr_in										_send_addr;
	struct sockaddr_in										_bc_addr;
	std::map<std::string /*ip*/, std::string /*answer*/>	_map_answer;

	void 				_init();

	bool				_send_question(std::string question);
	bool 				_listen_answer(int timeout);

};

#endif

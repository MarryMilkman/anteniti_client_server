#include <controllers/SSHTunnelController.hpp>
#include <controllers/RouterInfoController.hpp>
#include <controllers/SettingController.hpp>
#include <Parser.hpp>

SSHTunnelController::SSHTunnelController() {
	this->_listener = 0;
	this->_session = 0;
	this->_channel = 0;
	this->_sock = 0;
	this->_remote_listenaddr = "127.0.0.1";
	this->_remote_listenport = 1;
	// this->_initSSHTools();
}

SSHTunnelController::~SSHTunnelController() {

}

SSHTunnelController	&SSHTunnelController::getInstance() {
	static SSHTunnelController 	ssh_tc;

	std::cerr << "SSHTunnelController::getInstance(\n";
	return ssh_tc;
}

bool 			SSHTunnelController::refresh_tunnel() {
	this->disconnect_tunnel();
	return this->_initSSHTools();
}

std::map<std::string, std::string>	SSHTunnelController::get_instruction() {

	this->_data_from_channel = "";
	if (!this->_check_connection()) {
		std::cerr << "No connect...\n";
		throw std::exception();
	}
	if (this->_try_read_from_channel()) {
		std::vector<std::string> 	data_segments;

		data_segments = Parser::custom_split(this->_data_from_channel, "\n***DELIM***\n");
		if (data_segments.size() == 2) {
			std::vector<std::string> 			command_explain_segment = Parser::custom_split(data_segments[0], "-");
			std::map<std::string, std::string>	r_map;

			if (command_explain_segment[0] != "Command")
				throw std::exception();
			r_map["instruction"] = data_segments[1];
			//
			if (r_map["instruction"] == Constant::Comunicate::send_mac)
				return r_map;
			else if (command_explain_segment.size() != 2)
				throw std::exception();
			r_map["sn"] = command_explain_segment[1];
			return r_map;
			//

			// if (r_map["instruction"] != Constant::Comunicate::send_mac)
			// 	r_map["sn"] = command_explain_segment[1];
			// return r_map;
		}
			// throw std::exception();
	}
	throw std::exception();
	// return "";
}

bool 		SSHTunnelController::_try_read_from_channel() {
	char 	buff[64];
	int 	bytes = -1;

	this->_data_from_channel = "";
	while (bytes) {
		bytes = libssh2_channel_read(this->_channel, buff, sizeof(buff));
		buff[bytes] = 0;
		if (bytes == LIBSSH2_ERROR_EAGAIN || bytes == 0) {
			break;
		}
		if (bytes < 0) {
			this->_print_error("libssh2_channel_read");
			return false;
		}
		this->_data_from_channel += buff;
	}
	if (libssh2_channel_eof(this->_channel)) {
		std::cerr << "disconnect\n";

		this->_clean_channel();
	}
	if (this->_data_from_channel.size()) {
		std::cerr << this->_data_from_channel << " - MESSAGE!\n";
		return true;
	}

	return false;
}


bool 			SSHTunnelController::send_message(std::string message) {
	int 	bytes = 0;
	int 	len = message.size();

	std::cerr << "SSHTunnelController: try send_message: " << message << "\n";
	if (!this->_check_connection()) {
		std::cerr << "fail send message by ssh tunnel\n";
		return false;
	}
	while (len - bytes > 0) {
		int i = libssh2_channel_write(this->_channel, message.c_str() + bytes, len - bytes);

		if (i < 0) {
			this->_print_error("libssh2_channel_write");
			// this->_clean_channel();
			this->_initSSHTools();
			return false;
		}
		bytes += i;
	}
	return true;
}

void 	SSHTunnelController::disconnect_tunnel() {
	if (this->_channel) {
		libssh2_channel_free(this->_channel);
		this->_channel = 0;
	}
    if (this->_listener) {
		libssh2_channel_forward_cancel(this->_listener);
		this->_listener = 0;
	}
	if (this->_session) {
		libssh2_session_disconnect(this->_session, "Client disconnecting normally");
		libssh2_session_free(this->_session);
		this->_session = 0 ;
	}
	if (this->_sock > 0) {
		close(this->_sock);
		this->_sock = 0;
	}
}





bool 		SSHTunnelController::_check_connection() {
	if (!this->_session) {
		std::cerr << "sesion not init\n";

		int 		result_init = this->_initSSHTools();
		std::cerr << "result_init = " << result_init << "\n";
		if (result_init || !this->_session)
			return false;
	}


	if (!this->_listener) {
		std::cerr << "listener not init\n";
		// int 	try_port = -1;

		// while (++try_port < 65536 && !this->_listener)
		this->_listener = libssh2_channel_forward_listen_ex(this->_session,
							this->_remote_listenaddr.c_str(), 0, &this->_remote_listenport, 1);

		if (!this->_listener) {
			this->_print_error("libssh2_channel_forward_listen_ex");
			return false;
		}

		std::cerr << "SSHTunnelController: _remote_listenport: " << this->_remote_listenport << "\n";
	}
	if (!this->_channel) {
		// std::cerr << "channel not init\n";

		if (!this->_refresh_channal())
			return false;
		libssh2_session_set_blocking(this->_session, 0);
	}
	return true;
}

// MARK : private functions........
int 		SSHTunnelController::_initSSHTools() {
	struct 	sockaddr_in addr;
	int 	z = 1;

	this->disconnect_tunnel();
	if (!this->_sock)
		this->_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(22);
	// O_NONBLOCK
	// setsockopt(this->_sock, SOL_SOCKET, SO_RCVTIMEO, &z, sizeof(z));
	setsockopt(this->_sock, IPPROTO_TCP, TCP_SYNCNT, &z, sizeof(z));
	// fcntl(this->_sock, F_SETFL, O_NONBLOCK);
	if (INADDR_NONE == (addr.sin_addr.s_addr = inet_addr(Constant::SSHTunnel::kvm_service_ip.c_str()))) {
		fprintf(stderr, "Invalid remote IP address\n");
		close(this->_sock);
		this->_sock = 0;
		return -1;
	}
	// fd_set          readfds;
	// struct timeval  timeout;
	//
	// timeout.tv_sec = 3;
	// timeout.tv_usec = 0;
	// FD_ZERO(&readfds);
	// FD_SET(this->_sock , &readfds);
	// int activity = select(this->_sock + 1, &readfds, 0, 0, &timeout);
	// if (activity <= 0) {
	// 	std::cerr << "connect timeout\n";
	// 	return -1;
	// }
	if (connect(this->_sock, (struct sockaddr*)(&addr),
				sizeof(struct sockaddr_in)) != 0) {
		std::cerr << "Fail connect socket...\n";
		if (this->_sock)
			close(this->_sock);
		this->_sock = 0;
		return -1;
	}
	this->_session = libssh2_session_init();
	if (!this->_session) {
		this->_print_error("libssh2_session_init");
		close(this->_sock);
		this->_sock = 0;
		return -1;
	}
	if (libssh2_session_handshake(this->_session, this->_sock)) {
		this->_print_error("libssh2_session_handshake");
		close(this->_sock);
		this->_sock = 0;
		return -1;
	}
// AUTHORIZATION ---------------------------------------------------------------
	std::string 	username = Constant::SSHTunnel::ssh_tunnel_username;
	std::string 	pass = Constant::SSHTunnel::ssh_tunnel_password;
	std::string 	userauthlist;

	// libssh2_hostkey_hash(this->_session, LIBSSH2_HOSTKEY_HASH_SHA1);
	// userauthlist = libssh2_userauth_list(this->_session, username.c_str(), username.size());
	// std::cerr << "Authentication methods: " << userauthlist << "\n";
	if(libssh2_userauth_password(this->_session, username.c_str(), pass.c_str())) {
        this->_print_error("libssh2_userauth_password");
		this->disconnect_tunnel();
		return -1;
	}
	// libssh2_keepalive_config(this->_session, 1, 60);
	return 0;
}

bool 	SSHTunnelController::_refresh_channal() {
	libssh2_session_set_timeout(this->_session, 5000);
	if (!(this->_channel = libssh2_channel_forward_accept(this->_listener))) {
		// this->_print_error("libssh2_channel_forward_accept");
		// this->_clean_channel();
		return false;
	}
	// std::string 			intro_message;
	// RouterInfoController 	&info_controller = RouterInfoController::getInstance();
	// // int 					bytes = 0;
	// // int 					len;
	//
	// intro_message = "intro\n***DELIM***\n";
	// intro_message += info_controller.get_self_info().serial_number;
	// std::cerr << "try to send message:\n" << intro_message << "\n";
	// return this->send_message(intro_message);
	return true;
}





void 		SSHTunnelController::_write_setting_to_variable_file(std::string setting_str) {
	std::ofstream 	file(Constant::Files::variable_setting);

	file << setting_str;
}

void 		SSHTunnelController::_clean_channel() {
	if (this->_session)
		libssh2_session_set_blocking(this->_session, 1);
	if (this->_channel)
		libssh2_channel_free(this->_channel);
	this->_channel = 0;
}

int			SSHTunnelController::_print_error(std::string title) {
    char *errmsg;
    int errlen;
	int err;

	if (this->_session)
    	err = libssh2_session_last_error(this->_session, &errmsg, &errlen, 0);
	else {
		std::cerr << "Session not init! Cant use libssh2_session_last_error\n";
		return -1;
	}
    if (err)
        std::cerr << title << ": " << errmsg << "\n";
	return err;
}


void 			*SSHTunnelController::_callback() {
	SSHTunnelController 	&_ssh_tunnel_controller = SSHTunnelController::getInstance();

	_ssh_tunnel_controller._initSSHTools();
	return 0;
}

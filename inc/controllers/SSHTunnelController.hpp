#ifndef SSH_TUNNEL_CONTROLLER
#define SSH_TUNNEL_CONTROLLER

#include "lib.h"

// #define KVM_SERVICE_IP "10.0.0.12"
#define KVM_SERVICE_IP "91.145.198.4"

class SSHTunnelController {
	SSHTunnelController();
public:
	~SSHTunnelController();
	SSHTunnelController(SSHTunnelController const & ref) = delete;
	SSHTunnelController &operator=(SSHTunnelController const & ref) = delete;
	static SSHTunnelController	&getInstance();

	// bool 			is_tunnel_available();
	// bool 			make_tunnel();
	std::string 	get_instruction();
	bool 			send_message(std::string message);
	void 			disconnect_tunnel();

	void 	start();


private:
	int 				_sock;
	LIBSSH2_SESSION		*_session;
	LIBSSH2_LISTENER	*_listener;
	LIBSSH2_CHANNEL		*_channel;
	int 				_remote_listenport;
	std::string 		_remote_listenaddr;
	std::string 		_data_from_channel;


	int					_initSSHTools();

	bool 				_check_connection();

	bool				_try_read_from_channel();

	void 				_write_setting_to_variable_file(std::string setting_str);

	bool 				_refresh_channal();
	void 				_clean_channel();

	int 				_print_error(std::string mess);
	static void 		*_callback();
};

#endif

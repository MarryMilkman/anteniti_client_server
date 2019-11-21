#ifndef CONSTANT_HPP
# define CONSTANT_HPP

# include "iostream"

class Constant {
public:

	class Comunicate {
	public:
		static std::string 	incorrect_addressee;
		static std::string	nothing_to_chante;
		static std::string	message_delivered;
		static std::string	block_list_changed;
		static std::string 	blocklist_apply;
		static std::string	blocklist_not_apply;

		//
		static std::string	server_mod_lock;
		static std::string	server_mod_free;

		static std::string	send_mac;
		static std::string	send_info;

		static std::string	setting_changed;
		static std::string	setting_delivered;
		static std::string	setting_not_delivered;
		static std::string	apply_setting;
		static std::string	setting_applyed;
		static std::string	setting_not_applyed;
		static std::string	setting_rool_back;
		static std::string 	setting_save;

		static std::string	wan_changed;
		static std::string	wan_detect;

		static std::string	router_is_not_available;

	};
	class Blocking {
	public:
		static std::string	dit_blocklist;
		static std::string	path_blocklist;
	};
	class Broadcast {
	public:
		static std::string	bcc_addr;
		static int			bcc_port;
	};
	class Cloud {
	public:
		static std::string	apikey;
		static std::string	ssl_sert_url;
		static std::string	cloud_url;
		static std::string	sertificate_file_path;

		static std::string	notif_connect_new_d;
		static std::string	notif_connect_d;
		static std::string	notif_disconnect_d;
		static std::string	notif_jamming;
	};
	class Notification {
	public:
		static int 			notification_port;
		static int			critical_number_dis;
	};
	class SSHTunnel {
	public:
		static std::string	kvm_service_ip;
		static std::string	ssh_tunnel_password;
		static std::string	ssh_tunnel_username;
	};
	class ScriptExec {
	public:
		static std::string	script_path;
	};
	class TCP_IP {
	public:
		static int			listen_port;
	};
	class Files {
	public:
		static std::string			cloud_access_list;
		static std::string			access_list;

		static std::string			path_setting_scripts;
		static std::string			dir_setting;
		static std::string			setting;
		static std::string			copy_setting;
		static std::string			variable_setting;

		static std::string			dit_server_client_info;
		static std::string 			temp_sert;
		static std::string 			cloud_tmp_file;
		static std::string			connection_log;
		static std::string			ethernet_list;
		// static std::string 		ation::path_familiar_devices;
	};


};


#endif

#include "Constant.hpp"

// MARK: Comunicate

std::string 	Constant::Comunicate::incorrect_addressee	= "incorrect_addressee";

std::string		Constant::Comunicate::nothing_to_chante 	= "nothing_to_change";
std::string		Constant::Comunicate::message_delivered 	= "message_delivered";
std::string		Constant::Comunicate::block_list_changed 	= "block_list_changed";
std::string 	Constant::Comunicate::blocklist_apply		= "blocklist_apply";
std::string 	Constant::Comunicate::blocklist_not_apply 	= "blocklist_not_apply";

std::string		Constant::Comunicate::server_mod_lock 		= "server_mod_lock";
std::string		Constant::Comunicate::server_mod_free 		= "server_mod_free";

std::string		Constant::Comunicate::send_mac 				= "send_mac";
std::string		Constant::Comunicate::send_info 			= "send_info";

std::string		Constant::Comunicate::setting_changed 		= "setting_changed";
std::string		Constant::Comunicate::setting_delivered 	= "setting_delivered";
std::string		Constant::Comunicate::setting_not_delivered = "setting_not_delivered";
std::string		Constant::Comunicate::apply_setting 		= "apply_setting";
std::string		Constant::Comunicate::setting_applyed 		= "setting_applyed";
std::string		Constant::Comunicate::setting_not_applyed 	= "setting_not_applyed";
std::string		Constant::Comunicate::setting_rool_back 	= "setting_rool_back";
std::string	 	Constant::Comunicate::setting_save 			= "setting_save";

std::string		Constant::Comunicate::wan_changed 			= "wan_changed";
std::string		Constant::Comunicate::wan_detect 			= "wan_detect";

std::string	Constant::Comunicate::router_is_not_available 	= "router_is_not_available";

// MARK: Blocking


// MARK: Broadcast

std::string	Constant::Broadcast::bcc_addr 					= "192.168.99.255";
int 		Constant::Broadcast::bcc_port 					= 9390;

// MARK: Cloud
std::string 	Constant::Cloud::apikey 					= "11111111111111111111";
std::string 	Constant::Cloud::ssl_sert_url 				= "https://curl.haxx.se/ca/cacert.pem";
std::string 	Constant::Cloud::cloud_url 					= "https://www.anteniti.icu";
std::string 	Constant::Cloud::sertificate_file_path 		= "/home/user/Downloads/cacert.pem";

std::string 	Constant::Cloud::notif_connect_new_d 		= "0-001";
std::string 	Constant::Cloud::notif_connect_d 			= "0-002";
std::string 	Constant::Cloud::notif_disconnect_d 		= "0-003";
std::string 	Constant::Cloud::notif_jamming 				= "0-010";

// MARK: Notification

int 			Constant::Notification::notification_port 	= 9939;
int 			Constant::Notification::critical_number_dis = 3;


// MARK : SSHTunnel

std::string		Constant::SSHTunnel::kvm_service_ip 		= "192.168.1.179";
std::string		Constant::SSHTunnel::ssh_tunnel_username 	= "user";
std::string		Constant::SSHTunnel::ssh_tunnel_password 	= "11111111";

// MARK : ScriptExec

std::string		Constant::ScriptExec::script_path 			= "/root/scripts/";

// MARK : TCP_IP

int				Constant::TCP_IP::listen_port 				= 9742;

// MARK : Files

std::string 	Constant::Files::path_cloud_access_list 	= "/root/server_client_info/cloud_access_list.info";
std::string		Constant::Files::path_access_list 			= "/root/server_client_info/access_list.info";
std::string		Constant::Files::path_setting_scripts 		= "/root/scripts/";

std::string		Constant::Files::dit_server_client_info		= "/root/server_client_info/";
std::string 	Constant::Files::temp_sert_path 			= "/tmp/temp_sert.pem";
std::string 	Constant::Files::cloud_tmp_file_path 		= "/tmp/cloud_tmp_file.txt";
std::string		Constant::Files::connection_log		= "/root/server_client_info/connection_log.txt";
std::string		Constant::Files::dir_setting 				= "/root/server_client_info/setting/";
// std::string 	Constant::Notification::path_familiar_devices = "/root/server_client_info/familiar_devices.info";
std::string		Constant::Files::path_variable_setting 	= "/root/server_client_info/setting/variable_setting.system";
std::string		Constant::Files::path_setting 			= "/root/server_client_info/setting/setting.system";
std::string		Constant::Files::path_copy_setting 		= "/root/server_client_info/setting/copy_setting.system";

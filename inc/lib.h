#ifndef LIB_H
#define LIB_H


#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include <sys/wait.h>
#include <sys/prctl.h>

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <cstdarg>
#include <sys/select.h>
#include <mutex>
#include <thread>
#include <chrono>

#include <fcntl.h>

#include <json.h>

#include <libssh2.h>
#include <libssh2_sftp.h>
#include <curl/curl.h>

#define CONFIG_FILE_PATH "/tmp/server_client_config.txt"

#include "RouterData.hpp"
#include "Constant.hpp"


// std::vector<std::string>    custom_split(std::string str, std::string delim);



enum eBroadcastType
{
    bct_notify_setting = 0,
    bct_WAN_wasChange
};

// #define INCORRECT_ADDRESSEE "Incorrect_addressee"
//
// // define for comunicate:
//
// # define NOTHING_TO_CHANGE "Nothing_to_change"
// # define MESSAGE_DELIVERED "Message_delivered"
//
// # define BLOCKLIST_CHANGE "Block_list_changed"
//
// # define BLOCKLIST_APPLY "Blocklist_apply"
// # define BLOCKLIST_NOT_APPLY "Blocklist_not_apply"
//
// # define SERVER_MOD_LOCK "Server_mod_lock"
// # define SERVER_MOD_FREE "Server_mod_free"
//
// # define SEND_MAC "Send_mac"
// # define SEND_INFO "Send_info"
// // # define SEND_SETTING_VERSION "Send_setting_version"
//
// # define SETTING_CHANGED "Setting_changed"
//
// # define SETTING_DELIVERED "Setting_delivered"
// # define SETTING_NOT_DELIVERED "Setting_not_delivered"
//
// # define SETTING_APPLY "Apply_setting"
//
// # define SETTING_APPLYED "Setting_applyed"
// # define SETTING_NOT_APPLYED "Setting_not_applyed"
//
// # define SETTING_ROOL_BACK "Setting_rool_back"
//
//
// # define WAN_CHENGED "60"
// # define WAN_DETECT "61"
//
// # define ROUNER_NOT_AVAILABLE "Router_is_not_available"
//
// // # define KEY_WAS_CHANGED "Key_was_changed"
// // # define KEY_WAS_RESEIVED "Key_was_delivered"
//
// # define SETTING_SAVE "Setting_save"











#endif

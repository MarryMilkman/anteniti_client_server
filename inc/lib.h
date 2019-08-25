#ifndef LIB_H
#define LIB_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdarg>
#include <sys/select.h>
#include <libssh2.h>
#include <libssh2_sftp.h>
#include <curl/curl.h>

#define CONFIG_FILE_PATH "/tmp/server_client_config.txt"

#include "RouterData.hpp"


std::vector<std::string>    custom_split(std::string str, std::string delim);


enum eBroadcastType
{
    bct_notify_setting = 0,
    bct_WAN_wasChange
};

// define for comunicate:

#define SERVER_MOD_LOCK "Server mod lock"
#define SERVER_MOD_FREE "Server mod free"

#define SEND_INFO "Send info"
#define SEND_SETTING_VERSION "Send setting version"

#define SETTING_CHENGED "Setting chenged"

#define SETTING_DELIVERED "Setting delivered"
#define SETTING_NOT_DELIVERED "Setting not delivered"

#define SETTING_APPLY "Apply setting"

#define SETTING_APPLYED "Setting applyed"
#define SETTING_NOT_APPLYED "Setting not applyed"

#define SETTING_ROOL_BACK "Setting rool back"


#define WAN_CHENGED "60"
#define WAN_DETECT "61"

#define ROUNER_NOT_AVAILABLE "Router is not available"

#define KEY_WAS_CHANGED "Key was changed"
#define KEY_WAS_RESEIVED "Key was delivered"

#define SETTING_SAVE "Setting save"

#endif

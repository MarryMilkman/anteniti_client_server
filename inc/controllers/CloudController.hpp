#ifndef CLOUD_CONTROLLER_HPP
#define CLOUD_CONTROLLER_HPP

#include "lib.h"

#define APIKEY  "11111111111111111111"

#define TEMP_SERT_PATH "/tmp/temp_sert.pem"
#define SSL_SERT_URL "https://curl.haxx.se/ca/cacert.pem"
#define CLOUD_URL "https://www.anteniti.icu"
#define CLOUD_TMP_FILE_PATH "/tmp/cloud_tmp_file.txt"
#define SERTIFICATE_FILE_PATH "/home/user/Downloads/cacert.pem"


// NOTIFICATE:
#define NOTIF_CONNECT_NEW_D "0-001"
#define NOTIF_CONNECT_D "0-002"
#define NOTIF_DISCONNECT_D "0-003"
#define NOTIF_JAMMING "0-010"

// fild *coder*:
// 0-001 - connect new devices
// 0-002 - connect devices
// 0-003 - disconnect devices
// 0-010 - jamming

enum ePostType {
    forSend = 0,
    forGet
};

// typedef struct s_memoryStruct {
//     s_memoryStruct() {
//         this->memory = (char *)malloc(1);
//         this->size = 0;
//     }
//
//     ~s_memoryStruct() {
//         if (this->memory)
//             free(this->memory);
//     }
//
//     void        clean() {
//         if (this->memory)
//             free(this->memory);
// 		this->memory = (char *)malloc(1);
//         this->size = 0;
//     }
//
//     char        *memory;
//     size_t      size;
// }               t_memoryStruct;

// curl https://curl.haxx.se/ca/cacert.pem > qwe.pem

class CloudController {
    CloudController();
public:
    CloudController(CloudController const & ref) = delete;
    CloudController &operator=(CloudController const & ref) = delete;
    ~CloudController();
    static CloudController &getInstance();


	void 	notificat(std::string coder, std::string name);

    void    post_info_to_cloud(std::string info);
    void    get_setting_from_cloud();
	void 	get_blocklist_from_cloud();

    std::string     get_response();

private:
    std::mutex          _mutex;

    // CURL                *_curl;
	std::string 		_response;
    // t_memoryStruct      _response_mem;
    // t_memoryStruct      _ssl_certificate_mem;

    int                 _init_and_execute_post(std::string postfild, std::string url);
    // int                 _clean_after_post();
	static void			_prepare_message_for_curl_send(std::string &message);


    // int                 _get_ssl_sert();
    void                _pars_get_sert(std::string line);

    // static size_t       _writeMemoryCallback(void *contents,
    //                                       size_t size,
    //                                       size_t nmemb,
    //                                       void *userp);

    class PostFilds {
    public:
        PostFilds();
        PostFilds(PostFilds const & ref);
        PostFilds &operator=(PostFilds const & ref);
        ~PostFilds();

        std::string     action;
        std::string     serial_number;
        std::string     text;
        std::string     apikey;

		std::string 	name;
		std::string 	coder;

        std::string     get_postfilds(ePostType type);
		std::string 	get_postfilds_for_notification();
    private:
        std::string     _get_pf_for_send();
        std::string     _get_pf_for_get();
    };


};

#endif

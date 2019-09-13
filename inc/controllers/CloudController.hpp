#ifndef CLOUD_CONTROLLER_HPP
#define CLOUD_CONTROLLER_HPP

#include "lib.h"

#define APIKEY  "11111111111111111111"

#define TEMP_SERT_PATH "/tmp/temp_sert.pem"
#define SSL_SERT_URL "https://curl.haxx.se/ca/cacert.pem"
#define CLOUD_URL "https://www.anteniti.icu"
#define CLOUD_TMP_FILE_PATH "/tmp/cloud_tmp_file.txt"
#define SERTIFICATE_FILE_PATH "/home/user/Downloads/cacert.pem"

enum ePostType {
    forSendInfo = 0,
    forGetSetting
};

typedef struct s_memoryStruct {
    s_memoryStruct() {
        this->memory = (char *)malloc(1);
        this->size = 0;
    }

    ~s_memoryStruct() {
        if (this->memory)
            free(this->memory);
    }

    void        clean() {
        if (this->memory) {
            if (this->memory)
                free(this->memory);
            this->memory = (char *)malloc(1);
            this->size = 0;
        }
        this->size = 0;
    }

    char        *memory;
    size_t      size;
}               t_memoryStruct;

// https://www.anteniti.icu/?action=send-info&sn=01201905270000100001&text=[781]&apikey=11111111111111111111
// curl https://curl.haxx.se/ca/cacert.pem > qwe.pem

class CloudController {
    CloudController();
public:
    CloudController(CloudController const & ref) = delete;
    CloudController &operator=(CloudController const & ref) = delete;
    ~CloudController();
    static CloudController &getInstance();


    void    post_info_to_cloud(std::string info);
    void    get_setting_from_cloud();

    std::string     get_response();

private:
    std::mutex          _mutex;

    CURL                *_curl;
    t_memoryStruct      _response_mem;
    t_memoryStruct      _ssl_certificate_mem;

    int                 _init_and_execute_post(char *postfild, char *url);
    int                 _clean_after_post();



    int                 _get_ssl_sert();
    void                _pars_get_sert(std::string line);

    static size_t       _writeMemoryCallback(void *contents,
                                          size_t size,
                                          size_t nmemb,
                                          void *userp);

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

        std::string     get_postfilds(ePostType type);
    private:
        std::string     _get_pf_for_send_info();
        std::string     _get_pf_for_get_setting();
    };


};

#endif

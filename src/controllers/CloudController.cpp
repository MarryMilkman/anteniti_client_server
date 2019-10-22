#include "controllers/CloudController.hpp"
#include "controllers/RouterInfoController.hpp"
#include "controllers/SettingController.hpp"
#include "controllers/BlockingController.hpp"
#include "Parser.hpp"
#include "Mutex.hpp"

CloudController::CloudController() {
    curl_global_init(CURL_GLOBAL_ALL);
}

CloudController::~CloudController() {
    curl_global_cleanup();
}

CloudController &CloudController::getInstance() {
    static CloudController  cloud;

    return cloud;
}

int             CloudController::_get_ssl_sert() {
    CURLcode    res;

    if (this->_ssl_certificate_mem.size)
        return 0;
    this->_curl = curl_easy_init();
    if(this->_curl) {
        curl_easy_setopt(this->_curl, CURLOPT_URL, SSL_SERT_URL);
        curl_easy_setopt(this->_curl, CURLOPT_WRITEFUNCTION, CloudController::_writeMemoryCallback);
        curl_easy_setopt(this->_curl, CURLOPT_WRITEDATA, &this->_ssl_certificate_mem);
        curl_easy_setopt(this->_curl, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(this->_curl, CURLOPT_TIMEOUT, 3);

        // curl_easy_setopt(this->_curl, CURLOPT_VERBOSE, 1);

        res = curl_easy_perform(this->_curl);
        if(res != CURLE_OK) {
            std::cerr << res << " ";
            fprintf(stderr, "Get ssl sertificate failed: %s\n", curl_easy_strerror(res));
            this->_ssl_certificate_mem.clean();
            curl_easy_cleanup(this->_curl);
            this->_curl = 0;
            return -1;
        }
        curl_easy_cleanup(this->_curl);
    }
    return 0;
}



// MRAK : curl executers

void            CloudController::post_info_to_cloud(std::string info) {
	std::lock_guard<std::mutex> lg(this->_mutex);

	std::cerr << "cloud: send info\n";
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    CloudController::PostFilds  pf;

    pf.action = "send-info";
    pf.serial_number = RouterInfoController::getInstance().get_self_info().serial_number;
    pf.text = info;
    pf.apikey = APIKEY;

    if (this->_init_and_execute_post(pf.get_postfilds(ePostType::forSend).c_str(), CLOUD_URL)) {
        std::cerr << "Fail post_info_to_cloud\n";
        this->_clean_after_post();
        std::cerr << "end clean post_info_to_cloud\n";
        return ;
    }
    // std::cerr << "Send info: answer from cloud: " << this->_response_mem.memory << "\n";
    this->_clean_after_post();
}

void            CloudController::get_setting_from_cloud() {
    std::cout << "cloud: get setting\n";
	std::lock_guard<std::mutex> lg(this->_mutex);
    CloudController::PostFilds  pf;

    pf.action = "get-setting";
    pf.serial_number = RouterInfoController::getInstance().get_self_info().serial_number;
    pf.apikey = APIKEY;

    if (this->_init_and_execute_post((char *)pf.get_postfilds(ePostType::forGet).c_str(), (char *)CLOUD_URL)) {
        std::cerr << "Fail get_setting_from_cloud\n";
        this->_clean_after_post();
        return ;
    }
    this->_clean_after_post();

    std::ofstream               f_new_setting(PATH_VARIABLE_SETTING);
	std::cerr << this->_response_mem.memory << "\n";
    std::vector<std::string>    list_new_setting = Parser::pars_cloud_answer(std::string(this->_response_mem.memory));

    for (std::string setting : list_new_setting)
        f_new_setting << setting << "\n";
    f_new_setting.close();
    // std::cerr << "Get setting: answer from cloud: " << this->_response_mem.memory << "\n";
    // exit(0);
    this->_response_mem.clean();
}

void            CloudController::get_blocklist_from_cloud() {
    std::cout << "cloud: get blocklist\n";
	std::lock_guard<std::mutex> lg(this->_mutex);
    CloudController::PostFilds  pf;

    pf.action = "get-settingmac";
    pf.serial_number = RouterInfoController::getInstance().get_self_info().serial_number;
    pf.apikey = APIKEY;

    if (this->_init_and_execute_post((char *)pf.get_postfilds(ePostType::forGet).c_str(), (char *)CLOUD_URL)) {
        std::cerr << "Fail get_setting_from_cloud\n";
        this->_clean_after_post();
        return ;
    }
    this->_clean_after_post();

    std::ofstream               f_new_blocklist(PATH_BLOCKLIST);
    std::vector<std::string>    list_blocking_orders = Parser::pars_cloud_answer(std::string(this->_response_mem.memory));

    for (std::string setting : list_blocking_orders)
        f_new_blocklist << setting << "\n";
    f_new_blocklist.close();
    // std::cerr << "Get setting: answer from cloud: " << this->_response_mem.memory << "\n";
    // exit(0);
    this->_response_mem.clean();
}

void 		CloudController::notificat(std::string coder, std::string name) {
	std::cerr << "cloud: notificat " << coder << "\n";
	std::lock_guard<std::mutex> lg(this->_mutex);
    // std::this_thread::sleep_for(std::chrono::seconds(3));

    CloudController::PostFilds  pf;
	std::string 				str_pf;

    pf.action = "set-routpush";
    pf.serial_number = RouterInfoController::getInstance().get_self_info().serial_number;
    pf.name = name;
	pf.coder = coder;
    pf.apikey = APIKEY;
	str_pf = pf.get_postfilds_for_notification();
    if (this->_init_and_execute_post((char *)str_pf.c_str(), (char *)CLOUD_URL)) {
        std::cerr << "Fail CloudController::notificat\n";
        this->_clean_after_post();
        std::cerr << "end clean CloudController::notificat\n";
        return ;
    }
    std::cerr << "notificat: answer from cloud: " << this->_response_mem.memory << "\n";
    this->_clean_after_post();
}









int         CloudController::_init_and_execute_post(const char *postfild, const char *url) {
    std::cerr << "_init_and_execute_post!\n";
    std::ofstream               temp_file_sert;
    CURLcode                    res;

	////////////

	////////////
    this->_curl = 0;
    if (this->_get_ssl_sert())
        return -1;
	// std::cerr << postfild << "\n----------------------------------------------------------------\n";
    this->_response_mem.clean();
    temp_file_sert.open(TEMP_SERT_PATH);
    temp_file_sert << this->_ssl_certificate_mem.memory;
    temp_file_sert.close();
    this->_curl = curl_easy_init();
    if(this->_curl) {
        curl_easy_setopt(this->_curl, CURLOPT_URL, url);
        curl_easy_setopt(this->_curl, CURLOPT_POSTFIELDS,  postfild);
        curl_easy_setopt(this->_curl, CURLOPT_WRITEFUNCTION, _writeMemoryCallback);
        curl_easy_setopt(this->_curl, CURLOPT_WRITEDATA, (void *)&this->_response_mem);
        curl_easy_setopt(this->_curl, CURLOPT_CAINFO, TEMP_SERT_PATH);
        curl_easy_setopt(this->_curl, CURLOPT_TIMEOUT, 2);
        // curl_easy_setopt(this->_curl, CURLOPT_VERBOSE, 1);
        curl_easy_setopt(this->_curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(this->_curl, CURLOPT_SSL_VERIFYHOST, 2L);
        res = curl_easy_perform(this->_curl);
        if(res != CURLE_OK) {
            std::cerr << "Send info to cloud failed: " << curl_easy_strerror(res) << "\n";
            return -1;
        }
    }
    else
        return -1;
    return 0;
}

int         CloudController::_clean_after_post() {
    std::ofstream   temp_file_sert(TEMP_SERT_PATH);
    if (this->_curl)
        curl_easy_cleanup(this->_curl);
    return 0;
}


std::string     CloudController::get_response() {
    return this->_response_mem.memory;
}

    // callbeck function to story curl response
size_t          CloudController::_writeMemoryCallback(void *contents,
                                    size_t  size,
                                    size_t  nmemb,
                                    void    *userp)
{
    if (!userp) {
        (void)contents;
        (void)userp;
        return size * nmemb;
    }
    size_t realsize = size * nmemb;
    t_memoryStruct *mem = (t_memoryStruct *)userp;
    char *ptr = (char *)realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        std::cerr << "not enough memory (realloc returned NULL)\n";
        return 0;
    }
    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}



// MARK : PostFilds class -- PostFilds class -- PostFilds class -- PostFilds class

CloudController::PostFilds::PostFilds(){}

CloudController::PostFilds::PostFilds(PostFilds const & ref) {
    *this = ref;
}

CloudController::PostFilds &CloudController::PostFilds::operator=(PostFilds const & ref) {
    this->serial_number = ref.serial_number;
    this->text = ref.text;
    this->apikey = ref.apikey;
    this->action = ref.action;
    return *this;
}

CloudController::PostFilds::~PostFilds() {}

std::string CloudController::PostFilds::get_postfilds(ePostType type) {
    std::string pf;

    if (type == ePostType::forSend)
        pf = this->_get_pf_for_send();
    if (type == ePostType::forGet)
        pf = this->_get_pf_for_get();
    return pf;
}

std::string CloudController::PostFilds::get_postfilds_for_notification() {
	std::string postfild = "";

	if (this->coder == NOTIF_JAMMING) {
		postfild += "action=" + this->action + "&";
		postfild += "sn=" + this->serial_number + "&";
		postfild += "coder=" + this->coder + "&";
		postfild += "apikey=" + this->apikey;
		return postfild;
	}
	postfild += "action=" + this->action + "&";
    postfild += "sn=" + this->serial_number + "&";
    postfild += "name=" + this->name + "&";
    postfild += "coder=" + this->coder + "&";
	postfild += "apikey=" + this->apikey;
	return postfild;
}

std::string CloudController::PostFilds::_get_pf_for_send() {
    std::string postfilds = "";

	postfilds += "action=" + this->action + "&";
    postfilds += "sn=" + this->serial_number + "&";
    postfilds += "text=[" + this->text + "]&";
    postfilds += "apikey=" + this->apikey;
    return postfilds;
}

std::string CloudController::PostFilds::_get_pf_for_get() {
    std::string postfilds = "";

    postfilds += "action=" + this->action + "&";
    postfilds += "sn=" + this->serial_number + "&";
    postfilds += "apikey=" + this->apikey;
    return postfilds;
}

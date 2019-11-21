#include "controllers/CloudController.hpp"
#include "controllers/RouterInfoController.hpp"
#include "controllers/SettingController.hpp"
#include "ScriptExecutor.hpp"
#include "Parser.hpp"
#include "Mutex.hpp"

CloudController::CloudController() {
    // curl_global_init(CURL_GLOBAL_ALL);
}

CloudController::~CloudController() {
    // curl_global_cleanup();
}

CloudController &CloudController::getInstance() {
    static CloudController  cloud;

    return cloud;
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
    pf.apikey = Constant::Cloud::apikey;

    if (this->_init_and_execute_post(pf.get_postfilds(ePostType::forSend).c_str(), Constant::Cloud::cloud_url)) {
        std::cerr << "Fail post_info_to_cloud\n";
        return ;
    }
}

void            CloudController::get_setting_from_cloud() {
    std::cout << "cloud: get setting\n";
	std::lock_guard<std::mutex> lg(this->_mutex);
    CloudController::PostFilds  pf;

    pf.action = "get-setting";
    pf.serial_number = RouterInfoController::getInstance().get_self_info().serial_number;
    pf.apikey = Constant::Cloud::apikey;

    if (this->_init_and_execute_post(pf.get_postfilds(ePostType::forGet), Constant::Cloud::cloud_url)) {
        std::cerr << "Fail get_setting_from_cloud\n";
        return ;
    }
	std::cerr << this->_response << "\n";
    std::ofstream               f_new_setting(Constant::Files::variable_setting);

    f_new_setting << this->_response;
    f_new_setting.close();
}

void            CloudController::get_blocklist_from_cloud() {
    std::cout << "cloud: get blocklist\n";
	std::lock_guard<std::mutex> lg(this->_mutex);
    CloudController::PostFilds  pf;

    pf.action = "get-settingmac";
    pf.serial_number = RouterInfoController::getInstance().get_self_info().serial_number;
    pf.apikey = Constant::Cloud::apikey;

    if (this->_init_and_execute_post(pf.get_postfilds(ePostType::forGet), Constant::Cloud::cloud_url)) {
        std::cerr << "Fail get_setting_from_cloud\n";
        return ;
    }
    std::ofstream               f_new_blocklist(Constant::Files::cloud_access_list);

    f_new_blocklist << this->_response;
    f_new_blocklist.close();
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
    pf.apikey = Constant::Cloud::apikey;
	str_pf = pf.get_postfilds_for_notification();
    if (this->_init_and_execute_post(str_pf, Constant::Cloud::cloud_url)) {
        std::cerr << "Fail CloudController::notificat\n";
        std::cerr << "end clean CloudController::notificat\n";
        return ;
    }
}









int         CloudController::_init_and_execute_post(std::string postfild, std::string url) {
    std::cerr << "_init_and_execute_post!\n";
	std::string 	request = url + "/?" + postfild;

	CloudController::_prepare_message_for_curl_send(request);
	request = "\"" + request + "\"";
	// std::cerr << request << "\n";
	// std::cerr <<  "curl " << request.c_str() << "\n";
	this->_response = ScriptExecutor::getOutput::execute(2, "curl", request.c_str());
	// std::cerr << this->_response << "\n";
	return 0;

}

static void 	s_l_replacer(std::string &message, std::string replace, std::string search) {
	for(size_t pos = 0; ; pos += replace.size()) {
		pos = message.find(search, pos);
		if(pos == std::string::npos)
			break;
		message.erase(pos, search.size());
		message.insert(pos, replace);
	}
}

void 	CloudController::_prepare_message_for_curl_send(std::string &message) {
	s_l_replacer(message, "%20", " ");
	s_l_replacer(message, "\\[", "[");
	s_l_replacer(message, "\\]", "]");
	s_l_replacer(message, "\\{", "{");
	s_l_replacer(message, "\\}", "}");
	s_l_replacer(message, "\\\"", "\"");
}


std::string     CloudController::get_response() {
    return this->_response;
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

	if (this->coder == Constant::Cloud::notif_jamming) {
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
    postfilds += "text=" + this->text + "&";
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

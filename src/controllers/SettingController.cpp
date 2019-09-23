#include "controllers/SettingController.hpp"
#include "controllers/StatusController.hpp"
#include "controllers/CloudController.hpp"


#include "ScriptExecutor.hpp"
#include "Timer.hpp"
#include "Parser.hpp"

SettingController::SettingController()
{
    ScriptExecutor::getOutput::execute(1, "mkdir " DIR_SETTING);
    ScriptExecutor::getOutput::execute(1, "touch " PATH_SETTING);
    std::fstream    file(PATH_SETTING);
    std::string     version;

    file >> version;
    if (version.empty() || version != "Version") {
        file.close();
        std::ofstream   n_file_setting(PATH_SETTING);

        n_file_setting << "Version -1\n";
    }
    this->_init_list_setting();
}

SettingController::~SettingController() {}

SettingController &SettingController::getInstance() {
    std::lock_guard<std::mutex> guard(SettingController::_mutex);
    static SettingController    setting;

    return setting;
}

int                     SettingController::get_version() {
    std::lock_guard<std::mutex> guard(SettingController::_mutex);

    for (Setting setting : this->_list_setting)
    if (setting.option == "Version")
    return std::stoi(setting.value);
    return -1;
}

std::string             SettingController::get_str_unapply_options() {
    std::lock_guard<std::mutex> guard(SettingController::_mutex);

    std::string r_str;

    for (Setting unapply_setting : this->_list_unapply_setting)
        r_str += " " + unapply_setting.option;
    r_str = r_str.c_str() + 1;
    return r_str;
}




// MARK : Public function for change / story setting

    // create tmp_setting_for_roolback.system
    // copy some setting to tmp_setting_for_roolback.system
    // apply setting (start same scripts)..................
eSettingStatus         SettingController::apply_setting() {
    std::lock_guard<std::mutex> guard(SettingController::_mutex);

    std::vector<std::string>    list_apply_setting;
    std::vector<std::string>    list_fail_setting;
    // eWorkMod                    wm = StatusController::getInstance().getWorkMod();

    std::cerr << "setting: start APPLY!\n";
    // if (!this->_list_new_setting.size())
    //     if (!this->is_setting_chenge())
    //         return eSettingStatus::sApplyFail;
    this->_copy_old_setting();
    for (Setting setting : this->_list_new_setting) {
        int     answer;

        answer = this->_find_setting_end_apply(setting.option, setting.value);
        if (answer < 0) {
            std::cerr << "Not exist: " <<  setting.get_string() << "\n";
            continue;
        }
        if (answer > 0){
            std::cerr << "Setting fail: " << setting.get_string() << "\n";
            this->_list_unapply_setting.push_back(setting);
            continue;
        }
        list_apply_setting.push_back(setting.get_string());
    }
    if (this->_list_unapply_setting.size())
        return eSettingStatus::sApplyFail;
    if (list_apply_setting.size())
        this->_change_list_setting(list_apply_setting);
    return eSettingStatus::sApplyOK;
}

    // apply file tmp_setting_for_roolback.system
    // delete variable_setting.system
eSettingStatus         SettingController::roolback_setting(std::vector<std::string> list_unapply_options) {
    {
        std::lock_guard<std::mutex> guard(SettingController::_mutex);
        std::vector<std::string>    list_apply_setting;
        std::vector<std::string>    list_fail_setting;

        std::cerr << "setting: start ROOLBACK!\n";
        for (Setting cpy_setting : this->_list_copy_setting) {
            bool    is_exist = false;
            int     answer;

            for (std::string option : list_unapply_options)
                if (option == cpy_setting.option) {
                    is_exist = true;
                    break ;
                }
            if (!is_exist)
                continue;
            answer = this->_find_setting_end_apply(cpy_setting.option, cpy_setting.value);
            if (answer < 0)
                continue;
            if (answer > 0){
                std::cerr << "Setting fail: " << cpy_setting.get_string() << "\n";
                this->_list_unapply_setting.push_back(cpy_setting);
                continue;
            }
            list_apply_setting.push_back(cpy_setting.get_string());
        }
        if (!list_apply_setting.size()) {
            std::cerr << "Fail apply any setting\n";
            return eSettingStatus::sRoolbackFail;
        }
        this->_change_list_setting(list_apply_setting);
    }
    this->save_setting();
    return eSettingStatus::sRoolbackOK;
}

    // delete variable_setting.system
    // delete tmp_setting_for_roolbac   k.system
eSettingStatus         SettingController::save_setting() {
    std::lock_guard<std::mutex> guard(SettingController::_mutex);

    std::cerr << "setting: start SAVE!\n";
    std::ofstream   file(PATH_SETTING);

    for (Setting setting : this->_list_setting) {
        // std::cerr << setting.get_string() << "------<\n";
        file << setting.get_string() << "\n";
    }
    // exit(0);
    file.close();
    this->_delete_variable_file();
    this->_list_new_setting.clear();
    this->_list_copy_setting.clear();
    this->_list_unapply_setting.clear();

    return eSettingStatus::sSaveOK;
}




// MARK : check setting....

bool        SettingController::is_setting_chenge() {
    std::lock_guard<std::mutex> guard(SettingController::_mutex);

    eWorkMod        wm = StatusController::getInstance().getWorkMod();
    // static Timer    t;

    if (this->_list_new_setting.size()) {
        std::cerr << "List new setting alredy full\n";
        return true;
    }
    if (wm == eWorkMod::wm_server) {
	    if (this->_check_variable_file_setting()) {
	        return true;
	    } else {
			std::fstream 	file(PATH_VARIABLE_SETTING);

			if (file.is_open())
				system("rm " PATH_VARIABLE_SETTING);
		}
        CloudController::getInstance().get_setting_from_cloud();
        if (this->_check_variable_file_setting())
            return true;
        else
            this->_delete_variable_file();
    }
    else if (this->_check_variable_file_setting()) {
        return true;
    }
    return false;
}

    // _check_variable_file_setting - check variable_setting.system
    // check version - if old or no exist - return false
bool        SettingController::_check_variable_file_setting() {
    int             count_new_setting;
    eWorkMod        wm = StatusController::getInstance().getWorkMod();

    if (!this->_is_version_update()) {
        return false;
    }
    count_new_setting = this->_approve_new_setting();
    std::cerr << count_new_setting << " new setting.....\n";
    if (count_new_setting < 0 || (wm == eWorkMod::wm_server && !count_new_setting)) {
        return false;
    }
    return true;
}

bool        SettingController::_is_version_update() const {
    Setting             version = this->_list_setting[0];
    std::fstream        file_var;
    std::string         nv_option;
    std::string         nv_value;

    file_var.open(PATH_VARIABLE_SETTING);
    if (!file_var.is_open()) {
        return false;
    }
    if (StatusController::getInstance().getWorkMod() == eWorkMod::wm_client)
        return true;
    file_var >> nv_option;
    if (nv_option != version.option)
        return false;
    file_var >> nv_value;
    file_var.close();
    try {
        if (std::stoi(nv_value) > std::stoi(version.value)) {
            return true;
        }
    } catch (std::exception const & e) {}
    return false;
}

int         SettingController::_approve_new_setting() {
    std::vector<std::string>    list_geted_setting;
    std::fstream                file_var;
    std::string                 str_content_from_file;
    std::string                 line;
    eWorkMod                    work_mod = StatusController::getInstance().getWorkMod();
    int                         size;

    this->_list_new_setting.clear();
    file_var.open(PATH_VARIABLE_SETTING);
    if (!file_var.is_open())
        return -1;
    while(getline(file_var, line)){
        str_content_from_file += line + "\n";
    }
    file_var.close();
    list_geted_setting = Parser::pars_setting(str_content_from_file);
    size = list_geted_setting.size();
    for (Setting setting : this->_list_setting) {
        int i = 0;

        while (i < size) {
            if (list_geted_setting[i] == setting.get_string()) {
                list_geted_setting.erase(list_geted_setting.begin() + i);
                size = list_geted_setting.size();
                continue;
            }
            i++;
        }
        if (!size) {
            if (work_mod == eWorkMod::wm_server)
                return -1;
            return 0;
        }
    }
    for (std::string str : list_geted_setting) {
        this->_list_new_setting.push_back(Setting(str));
    }
    return this->_list_new_setting.size();
}




void    SettingController::_init_list_setting() {
    std::fstream    file(PATH_SETTING);
    std::string     line;

    while(getline(file, line))
        this->_list_setting.push_back(Setting(line));
}

int         SettingController::_change_list_setting(std::vector<std::string> new_list_setting) {
    std::string     new_cmp_option;

    for (Setting &setting : this->_list_setting) {
        int                 i = 0;

        for (std::string new_setting_str : new_list_setting) {
            std::stringstream ss(new_setting_str);

            ss >> new_cmp_option;
            if (setting.option == new_cmp_option) {
                setting.init_by_string(new_setting_str);
                new_list_setting.erase(new_list_setting.begin() + i);
                break;
            }
            i++;
        }
    }
    for (std::string new_setting_str : new_list_setting)
        this->_list_setting.push_back(Setting(new_setting_str));
    return 0;
}

int         SettingController::_copy_old_setting() {
    for (Setting setting : this->_list_setting){
        for (Setting new_sett : this->_list_new_setting){
            if (setting.value == new_sett.value && setting.option != new_sett.option)
                this->_list_copy_setting.push_back(setting);
        }
    }
    return this->_list_copy_setting.size();
}

void                    SettingController::_delete_variable_file() {
    std::fstream f(PATH_VARIABLE_SETTING);

    if (f.is_open()) {
        f.close();
        system("rm " PATH_VARIABLE_SETTING);
    }
}







// MARK : APPLY section

int     SettingController::_find_setting_end_apply(std::string setting, std::string value) {
    // make maaaany if else
    // and execute scripts by name of setting

    if (setting == "AdministratorAuthorizationBegin" ||
        setting == "AdministratorAuthorizationend" ||
        setting == "DevicesSystemBegin" ||
        setting == "DevicesSystemEnd" ||
        setting == "DevicesSettingBegin" ||
        setting == "DevicesSettingEnd" ||
        setting == "DeviceSecurityBegin" ||
        setting == "DeviceSecurityEnd" ||
        setting == "DeviceGroupBegin" ||
        setting == "DeviceGroupEnd" ||
        setting == "Version" ||
        setting.empty())
        return 0;

//////////////DeviceSystemBegin////////////////

    if (setting == "ISP") {
        // return exec_ISP(value);
    }
    else if (setting == "ISPIPaddress") {
        // return exec_ISPIPaddress(value);
    }
    else if (setting == "ISPRouterIP") {
        // return exec_ISPRouterIP(value);
    }
    else if (setting == "ISPSubnetMask") {
        // return exec_ISPSubnetMask(value);
    }
    else if (setting == "ISPIPv6Prefix") {
        // return exec_ISPIPv6Prefix(value);
    }
    else if (setting == "PPPoE") {
        // return exec_PPPoE(value);
    }
    else if (setting == "PPPoELogin") {
        // return exec_PPPoELogin(value);
    }
    else if (setting == "PPPoEPassword") {
        // return exec_PPPoEPassword(value);
    }
    else if (setting == "PPPoEMTU") {
        // return exec_PPPoEMTU(value);
    }
    else if (setting == "PPPoENameSys") {
        // return exec_PPPoENameSys(value);
    }
    else if (setting == "PPPoENameServ") {
        // return exec_PPPoENameServ(value);
    }
    else if (setting == "ISPDNS") {
        // return exec_ISPDNS(value);
    }
    else if (setting == "ISPDNSIPv4Primary") {
        // return exec_ISPDNSIPv4Primary(value);
    }
    else if (setting == "ISPDNSIPv4Secondary") {
        // return exec_ISPDNSIPv4Secondary(value);
    }
    else if (setting == "ISPDNSIPv6Primary") {
        // return exec_ISPDNSIPv6Primary(value);
    }
    else if (setting == "ISPDNSIPv6Secondary") {
        // return exec_ISPDNSIPv6Secondary(value);
    }
    else if (setting == "DHCP") {
        // return exec_DHCP(value);
    }
    else if (setting == "Bridge") {
        // return exec_Bridge(value);
    }
    else if (setting == "DHCPSubnetIP") {
        // return exec_DHCPSubnetIP(value);
    }
    else if (setting == "DHCPSubnetMask") {
        // return exec_DHCPSubnetMask(value);
    }
    else if (setting == "DHCPStartingIP") {
        // return exec_DHCPStartingIP(value);
    }
    else if (setting == "DHCPEndingIP") {
        // return exec_DHCPEndingIP(value);
    }
    else if (setting == "UPnP") {
        // return exec_UPnP(value);
    }
//////////////DevicesSettingBegin///////////////
    else if (setting == "WifiName") {
        // return exec_WifiName(value);
    }
    else if (setting == "WifiPass") {
        // return exec_WifiPass(value);
    }
    else if (setting == "WifiGuest") {
        // return exec_WifiGuest(value);
    }
    else if (setting == "WifiGuestName") {
        // return exec_WifiGuestName(value);
    }
    else if (setting == "WifiGuestPass") {
        // return exec_WifiGuestPass(value);
    }
    else if (setting == "WifiGuestTimeStart") {
        // return exec_WifiGuestTimeStart(value);
    }
    else if (setting == "WifiGuestTimeEnd") {
        // return exec_WifiGuestTimeEnd(value);
    }
    else if (setting == "WifiGuestDay") {
        // return exec_WifiGuestDay(value);
    }
//////////////DeviceSecurityBegin///////////////

    else if (setting == "QoS") {
        // return exec_QoS(value);
    }
    else if (setting == "AdBlock") {
        // return exec_AdBlock(value);
    }
    else if (setting == "Stealth") {
        // return exec_Stealth(value);
    }
    else if (setting == "BNA") {
        // return exec_BNA(value);
    }
    else if (setting == "CAGN") {
        // return exec_CAGN(value);
    }
    else
        return this->_apply_group_setting(setting, value);
    return 0;
}

int     SettingController::_apply_group_setting(
                        std::string setting,
                        std::string value)
{
    std::string     param;
    std::string     nbr_string;
    int             nbr;
    int             index;

    index = setting.find_first_of("1234567890");
    if (index == -1)
        return -1;
    nbr_string = (setting.c_str() + index);
    try {
        nbr = std::stoi(nbr_string);
        if (std::to_string(nbr) != nbr_string)
            return -1;
    } catch (std::exception &e) {
        return -1;
    }
    setting[index] = 0;
    param = setting.c_str();

    if (value.empty())
        (void)value;

    if (param == "GroupName") {
        // return exec_GroupName(nbr_string, value);
    }
    else if (param == "GroupMac") {
        // return exec_GroupMac(nbr_string, value);
    }
    else if (param == "GroupAccess") {
        // return exec_GroupAccess(nbr_string, value);
    }
    else if (param == "GroupTimeStart") {
        // return exec_GroupTimeStart(nbr_string, value);
    }
    else if (param == "GroupTimeEnd") {
        // return exec_GroupTimeEnd(nbr_string, value);
    }
    else if (param == "GroupDay") {
        // return exec_GroupDay(nbr_string, value);
    }
    else
        return -1;
    return 0;
}

std::mutex      SettingController::_mutex;

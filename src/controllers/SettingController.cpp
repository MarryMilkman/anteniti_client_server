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
}

SettingController::~SettingController() {}

SettingController &SettingController::getInstance() {
    static SettingController setting;

    return setting;
}






// MARK : Public function for change / story setting

    // create tmp_setting_for_roolback.system
    // copy some setting to tmp_setting_for_roolback.system
    // apply setting (start same scripts)..................
eSettingStatus         SettingController::apply_setting() {
    // std::fstream   file;
    std::string    line;
    std::vector<std::string>    list_apply_setting;
    std::vector<std::string>    list_fail_setting;

    // if (this->_type_setting == eSettingType::tGlobal)
        // file.open(PATH_SETTING);
    // else

        std::cerr << "setting: start APPLY!\n";
    // file.open(PATH_VARIABLE_SETTING);
    // if (!file.is_open()) {
    //     std::cerr << "apply fail! 1\n";
    //     return eSettingStatus::sApplyFail_no_file;
    // }
    this->_copy_old_setting();
    for (Setting obj_setting : this->_list_setting) {
        if (this->_find_setting_end_apply(obj_setting.option, obj_setting.parameter)){
            std::cerr << "apply fail! " << line << "\n";
            list_fail_setting.push_back(obj_setting.get_string());
            continue;
        }
        list_apply_setting.push_back(obj_setting.get_string());
    }
    this->_change_setting_file(list_apply_setting);
    return eSettingStatus:: sApplyOK;
}

    // apply file tmp_setting_for_roolback.system
    // delete variable_setting.system
eSettingStatus         SettingController::roolback_setting() {
    std::fstream   file;
    std::string    line;
    std::vector<std::string>    list_apply_setting;
    std::vector<std::string>    list_fail_setting;

    std::cerr << "setting: start ROOLBACK!\n";
    file.open(PATH_COPY_SETTING);
    if (!file.is_open()) {
        std::cerr << "roolback fail! 1\n";
        return eSettingStatus::sRoolbackFail;
    }
    while(getline(file, line)) {
        if (line[0] == '#')
            continue;
        std::string     option;
        std::string     parameter;
        this->_pars_line(line, option, parameter);
        if (this->_find_setting_end_apply(option, parameter)){
            std::cerr << "roolback fail! " << line << "\n";
            list_fail_setting.push_back(line);
            continue;
            // << ":" << parameter << "\n";
            // return eSettingStatus::sRoolbackFail;
        }
        list_apply_setting.push_back(line);
    }
    if (!list_apply_setting.size()) {
        std::cerr << "Fail apply any setting\n";
        return eSettingStatus::sRoolbackFail;
    }
    this->_change_setting_file(list_apply_setting);
    this->save_setting();
    return eSettingStatus::sRoolbackOK;
}

    // delete variable_setting.system
    // delete tmp_setting_for_roolbac   k.system
eSettingStatus         SettingController::save_setting() {
    std::cerr << "setting: start SAVE!\n";
    system("rm " PATH_VARIABLE_SETTING);
    return eSettingStatus::sSaveOK;
}















// MARK : check setting....

bool        SettingController::is_setting_chenge() {
    eWorkMod    wm = StatusController::getInstance().getWorkMod();

    if (this->_list_new_setting.size())
        return true;
    if (wm == eWorkMod::wm_server) {
        if (this->_check_local_setting()) {
            std::cerr << "Setting chenge!\n";
            return true;
        }
        if (this->_load_and_check_cloud_setting()) {
            std::cerr << "Setting chenge!\n";
            return true;
        }
    }
    else if (this->_check_local_setting()) {
        std::cerr << "Setting chenge!\n";
        return true;
    }
    return false;
}

    // load setting from cloud to /tmp/setting/cloud/download_setting.system
    // compare /tmp/setting/setting.system and privios_download.system ...
    //      if cant find line of download_setting in setting - retufn true
    //      else return false
bool        SettingController::_load_and_check_cloud_setting() {
    static Timer   t;

    if (t.one_time_in(2)) {
        CloudController::getInstance().get_setting_from_cloud();
        return this->_check_local_setting();

    }
    return false;
}

    // try open file /tmp/setting/variable_setting.system
    //      if file exist - return true
    //      else return false
bool        SettingController::_check_local_setting() {
    int             count_new_setting;
    eWorkMod        wm = StatusController::getInstance().getWorkMod();

    if (!this->_is_version_update()) {
        return false;
    }
    count_new_setting = this->_approve_new_setting();
    if (count_new_setting < 0 || (wm == eWorkMod::wm_server && !count_new_setting))
        return false;
    this->_type_setting = eSettingType::tLocal;
    return true;
    // if (this->_check_time != this->_get_time_midify())
    //     return true;
    // return false;
}

bool        SettingController::_is_version_update() {
    std::fstream    file_var;
    std::fstream    file_set;
    std::string     version_var;
    std::string     version_set;
    std::stringstream   ss;

    file_var.open(PATH_VARIABLE_SETTING);
    file_set.open(PATH_SETTING);
    if (!file_var.is_open() || !file_set.is_open()) {
        return false;
    }
    file_var >> version_var;
    file_set >> version_set;
    if (version_set != version_var)
        return false;
    file_var >> version_var;
    file_var.close();
    file_set >> version_set;
    file_set.close();
    try {
        if (std::stoi(version_var) > std::stoi(version_set)) {
            return true;
        }
    } catch (std::exception const & e) {}
    system("rm " PATH_VARIABLE_SETTING);
    return false;
}

int         SettingController::_approve_new_setting() {
    std::fstream                file_var;
    std::fstream                file_set;
    std::vector<std::string>    list_geted_setting;
    std::string                 str_content_from_file;
    std::string                 line;

    file_var.open(PATH_VARIABLE_SETTING);
    file_set.open(PATH_SETTING);
    if (!file_var.is_open() || !file_set.is_open())
        return -1;
    while(getline(file_var, line)){
        str_content_from_file += line + "\n";
    }
    list_geted_setting = Parser::pars_setting(str_content_from_file);
    file_var.close();
    int     size = list_geted_setting.size();
    int     i;

    while (getline(file_set, line)) {
        if (!line.size())
            continue;
        i = 0;
        while (i < size) {
            if (list_geted_setting[i] == line) {
                list_geted_setting.erase(list_geted_setting.begin() + i);
                size = list_geted_setting.size();
                continue;
            }
            i++;
        }
        if (!list_geted_setting.size()) {
            if (StatusController::getInstance().getWorkMod() == eWorkMod::wm_server) {
                system("rm " PATH_VARIABLE_SETTING);
                return -1;
            }
            std::ofstream   new_file_var(PATH_VARIABLE_SETTING);
            return 0;
        }
    }
    file_set.close();
    system("rm " PATH_VARIABLE_SETTING);
    this->_list_setting.clear();
    for (std::string n_setting : list_geted_setting)
        this->_list_setting.push_back(Setting(n_setting));
    // std::ofstream   new_file_var(PATH_VARIABLE_SETTING);
    //
    // for (std::string option : list_geted_setting) {
    //     new_file_var << option << "\n";
    // }
    // new_file_var.close();
    // std::cerr << "Scan make... " << list_geted_setting.size() << " new setting\n";
    return this->_list_setting.size();
}


int         SettingController::_change_setting_file(std::vector<std::string> new_list_setting) {
    std::fstream                set_file(PATH_SETTING);
    std::vector<std::string>    old_list_setting;
    std::string                 line;

    while (getline(set_file, line)) {
        if (line.empty())
            continue;
        old_list_setting.push_back(line);
    }
    set_file.close();

    std::string     cmp_option;
    std::string     re_cmp_option;

    std::ofstream   new_set_file(PATH_SETTING);

    // for (std::string re_l : re_list_option)
    //     std::cerr << re_l << "----<\n";


    for (std::string &l : old_list_setting) {
        std::stringstream   ss(l);
        int                 i = 0;

        ss >> cmp_option;
        for (std::string re_l : new_list_setting) {
            std::stringstream re_ss(re_l);

            re_ss >> re_cmp_option;
            if (cmp_option == re_cmp_option) {
                l = re_l;
                new_list_setting.erase(new_list_setting.begin() + i);
                break;
            }
            i++;
        }
        new_set_file << l << "\n";
    }

    for (std::string re_l : new_list_setting)
        new_set_file << re_l << "\n";
    new_set_file.close();
    return 0;
}


int         SettingController::_copy_old_setting() {
    std::fstream                file_set;
    std::vector<std::string>    list_copy_option;
    std::string                 line;


    file_set.open(PATH_SETTING);
    if (!file_set.is_open())
        return -1;
    while (getline(file_set, line)) {
        if (line.empty())
            continue;
        std::stringstream   ss(line);
        std::string         cmp_file_param;

        // std::cerr << line << "??\n";

        ss >> cmp_file_param;
        for (Setting new_sett : this->_list_setting){
            std::stringstream ss(new_o);
            std::string       smp_new_str;

            ss >> smp_new_str;
            if (cmp_file_param == smp_new_str && new_o != line) {
                // std::cerr << new_o << " problem?\n";
                list_copy_option.push_back(line);
            }
        }
    }

    file_set.close();
    std::ofstream   file_copy;

    file_copy.open(PATH_COPY_SETTING);
    for (std::string option : list_copy_option)
            file_copy << option << "\n";
    file_copy.close();

    return list_copy_option.size();
}

//
// time_t      SettingController::_get_time_midify() {
//     struct stat buff;
//
//     stat(this->path_to_variable_setting.c_str(), &buff);
//     return buff.st_mtime;
// }









int     SettingController::_find_setting_end_apply(std::string setting, std::string value) {
    // make meeeeeny if else
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

// void        SettingController::_pars_line(std::string line, std::string &option, std::string &param) {
//     std::stringstream ss;
//
//     line[line.find_first_of("#")] = 0;
//     ss << line.c_str();
//     ss >> option;
//     param = (line.c_str() + line.size() + 1);
// }

#ifndef SETTING_CONTROLLER
# define SETTING_CONTROLLER

// #define DIR_SETTING "/tmp/setting/"
//
// #define PATH_VARIABLE_SETTING "/tmp/setting/variable_setting.system"
// #define PATH_SETTING "/tmp/setting/setting.system"
// #define PATH_COPY_SETTING "/tmp/setting/copy_setting.system"
//
//
// #define PATH_SETTING_SCRIPTS "/root/scripts/"

# include "lib.h"
# include "controllers/StatusController.hpp"
# include "controllers/CloudController.hpp"
# include "controllers/setting_tools/Setting.hpp"

enum eSettingStatus {
    sApplyOK,
    sApplyFail_no_file,
    sApplyFail_no_new_setting,
    sApplyFail,
    sRoolbackOK,
    sRoolbackFail,
    sSaveOK
};

class SettingController {
    SettingController();
public:
    SettingController(SettingController const &ref) = delete;
    SettingController &operator=(SettingController const &ref) = delete;
    ~SettingController();
    static SettingController    &getInstance();
    std::string                 get_str_unapply_options();
    // int                         get_version();

    eSettingStatus  apply_setting();
    eSettingStatus  roolback_setting(std::vector<std::string> list_unapply_options);
    eSettingStatus  save_setting();

    bool            is_setting_chenge();

private:
    // static std::mutex       _mutex;
	StatusController			&_status_controller;
	CloudController 			&_cloud_controller;

    std::vector<Setting>    _list_new_setting;
    std::vector<Setting>    _list_setting;
    std::vector<Setting>    _list_copy_setting;
    std::vector<Setting>    _list_unapply_setting;
	bool 					_need_reload;

    void                _init_list_setting();
    int                 _copy_old_setting();
    int                 _change_list_setting(std::vector<std::string> new_list_setting);

    void                _delete_variable_file();

// Setting CHEKER:
    bool                _check_variable_file_setting();
    // bool                _is_version_update() const;
    int                 _approve_new_setting();

// APPLY:
    int                 _find_setting_end_apply(std::string setting, std::string value);
    int                 _apply_group_setting(std::string setting, std::string value);

// Setting execution wrapper

	int 				_execution_wrapper_WiFiName(std::string value);
	int 				_execution_wrapper_WiFiPass(std::string value);
	int   				_execution_wrapper_Stealth(std::string value);
	int 				_execution_wrapper_Vlan(std::string value);

};

#endif

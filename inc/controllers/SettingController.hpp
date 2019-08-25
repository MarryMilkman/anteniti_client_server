#ifndef SETTING_CONTROLLER
# define SETTING_CONTROLLER

#define DIR_SETTING "/tmp/setting/"

#define PATH_VARIABLE_SETTING "/tmp/setting/variable_setting.system"
#define PATH_SETTING "/tmp/setting/setting.system"
#define PATH_COPY_SETTING "/tmp/setting/copy_setting.system"

# include "lib.h"
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

enum eSettingType {
    tLocal = 0,
    tCloud,
    tGlobal
};

class SettingController {
    SettingController();
public:
    SettingController(SettingController const &ref) = delete;
    SettingController &operator=(SettingController const &ref) = delete;
    ~SettingController();
    static SettingController &getInstance();

    eSettingStatus  apply_setting();
    eSettingStatus  roolback_setting();
    eSettingStatus  save_setting();

    bool            is_setting_chenge();

    std::vector<Setting>    &get_list_unapply_setting();

private:
    // time_t 				_check_time;
    std::vector<Setting>    _list_new_setting;
    std::vector<Setting>    _list_setting;
    std::vector<Setting>    _list_copy_setting;
    std::vector<Setting>    _list_unapply_setting;
    eSettingType            _type_setting;

    bool                _check_local_setting();
    bool                _load_and_check_cloud_setting();

    int                 _find_setting_end_apply(std::string setting, std::string value);
    int                 _apply_group_setting(std::string setting, std::string value);

    int                 _copy_old_setting();
    int                 _approve_new_setting();
    int                 _change_setting_file(std::vector<std::string> new_list_setting);

    bool                _is_version_update();

    void                _pars_line(std::string line, std::string &option, std::string &param);
};

#endif

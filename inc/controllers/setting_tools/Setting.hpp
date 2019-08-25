#ifndef SETTING_HPP
#define SETTING_HPP

#include "lib.h"

class Setting {
public:
    Setting();
    Setting(std::string str);
    Setting(std::string opt, std::string param);
    Setting(Setting const &ref);
    Setting &operator=(Setting const &ref);
    ~Setting();

    std::string     option;
    std::string     parametr;

    int             init_by_setting(std::string str);
    std::string     get_string();
};

#endif

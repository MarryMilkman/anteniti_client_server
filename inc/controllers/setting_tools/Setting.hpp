#ifndef SETTING_HPP
#define SETTING_HPP

#include "lib.h"

class Setting {
public:
    Setting();
    Setting(std::string str);
    Setting(std::string opt, std::string value);
    Setting(Setting const &ref);
    Setting &operator=(Setting const &ref);
    ~Setting();

    std::string     option;
    std::string     value;

    int             init_by_string(std::string str);
    std::string     get_string();
};

#endif

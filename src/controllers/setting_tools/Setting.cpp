#include "controllers/setting_tools/Setting.hpp"

Setting::Setting() {
}

Setting::Setting(std::string str) {
    this->init_by_string(str);
}


Setting::Setting(std::string opt, std::string value) :
    option(opt), value(value)
{
}

Setting::Setting(Setting const & ref) {
    *this = ref;
}

Setting::~Setting() {
}

Setting     &Setting::operator=(Setting const &ref) {
    this->option = ref.option;
    this->value = ref.value;
    return *this;
}

int         Setting::init_by_string(std::string str) {
    std::stringstream   ss(str);
    std::string         word;

    ss >> this->option;
    ss >> this->value;
    while (ss >> word)
        this->value += " " + word;
    if (!this->option.empty() || !this->value.empty())
        return -1;
    return 0;
}

std::string Setting::get_string() {
    std::string     r_str;


    r_str = this->option;
    if (!this->value.empty())
        r_str += " " + this->value;
    return r_str;
}

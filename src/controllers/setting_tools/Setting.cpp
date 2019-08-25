#include "controllers/setting_tools/Setting.hpp"

Setting::Setting() {
}

Setting::Setting(std::string str) {
    this->init_by_setting(str);
}


Setting::Setting(std::string opt, std::string param) :
    option(opt), parametr(param)
{
}

Setting::Setting(Setting const & ref) {
    *this = ref;
}

Setting::~Setting() {
}

Setting     &Setting::operator=(Setting const &ref) {
    this->option = ref.option;
    this->parametr = ref.parametr;
    return *this;
}

int         Setting::init_by_setting(std::string str) {
    std::stringstream   ss(str);

    ss >> this->option;
    getline(ss, this->parametr);
    if (!this->option.empty() || !this->parametr.empty())
        return -1;
    return 0;
}

std::string Setting::get_string() {
    return this->option + " " + this->parametr;
}

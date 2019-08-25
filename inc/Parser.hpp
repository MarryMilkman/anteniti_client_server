#ifndef PARSER
#define PARSER

#include "lib.h"

class Parser {
public:
    Parser();
    ~Parser();

    void            pars_and_add_to_answer(std::string message);
    std::string     get_answer_for_cloud();

    static std::vector<std::string> pars_setting(std::string setting_str);

private:
    std::vector<std::string>    _list_str_info;

};

#endif

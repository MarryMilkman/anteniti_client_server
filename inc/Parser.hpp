#ifndef PARSER
#define PARSER

#include "lib.h"

class Parser {
public:
    Parser();
    ~Parser();


    void            pars_and_add_to_answer(std::string message);
    std::string     get_answer_for_cloud();

	static std::vector<std::string> custom_split(std::string str, std::string delim);
    static std::vector<std::string> pars_cloud_answer(std::string setting_str);
    static std::vector<std::string> pars_answer_apply(std::string str_with_answer);
	class Info {
	public:
		static std::map<std::string, std::string>	pars_sysinfo(std::string std_sysinfo);
	};
private:
    std::vector<std::string>    _list_str_info;

};

#endif

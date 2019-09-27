#include "Parser.hpp"

Parser::Parser() {
}

Parser::~Parser() {
}

void        Parser::pars_and_add_to_answer(std::string message) {
    std::string     str_info;

    str_info += "\n*******BEGIN INFO*******\n";
    str_info += message;
    str_info += "\n*******END INFO*******\n";
    this->_list_str_info.push_back(str_info);
}

std::string Parser::get_answer_for_cloud() {
    std::string     answer;

    for (std::string str : this->_list_str_info)
        answer += str;
    return answer;
}

std::vector<std::string> Parser::custom_split(std::string str, std::string delim) {
	int 						i;
	std::vector<std::string>	r_list;
	std::string 				prom_str;


	while (str.size()) {
		i = str.find(delim);
		if (i < 0) {
			if (!str.empty())
			r_list.push_back(str);
			break;
		}
		str[i] = 0;
		if (i)
			r_list.push_back(str.c_str());
		str = (str.c_str() + i + delim.size());
	}
	return r_list;
}

std::vector<std::string>    Parser::pars_cloud_answer(std::string setting_str) {
    std::vector<std::string>    r_list;
    int                         i = 0;
    int                         size;

    if (setting_str.find("<br/>") > setting_str.size())
        r_list = custom_split(setting_str, "\n");
    else
        r_list = custom_split(setting_str, "<br/>");
    size = r_list.size();
    while (i < size) {
        std::stringstream           ss;
        std::string                 &setting = r_list[i];
        std::string                 fragment;

        ss << setting;
        setting = "";
        ss >> setting;
        while (ss >> fragment)
            setting += " " + fragment;
        if (setting.empty()) {
            r_list.erase(r_list.begin() + i);
            size = r_list.size();
            continue;
        }
        i++;
    }
    // r_list = this->_psfc_get_list_setting_without_comment(setting_str);
    size = r_list.size();
    i = -1;
    while (++i < size) {
        int     j = 0;

        while (j < size) {
            if (i != j && r_list[i] == r_list[j]) {
                r_list.erase(r_list.begin() + j);
                size = r_list.size();
            }
            j++;
        }
    }

// check -----------------
    // for (std::string l : r_list)
    //         std::cerr << l << "*\n";
    // std::cerr << "I vsia??????????\n";
    // exit(0);
/////////////////////////////////////////
    return r_list;
}

std::vector<std::string>    Parser::pars_answer_apply(std::string str_with_answer) {
    std::vector<std::string>    general_list_unapply_options;
    std::vector<std::string>    list_lines = custom_split(str_with_answer, "\n");

    for (std::string line : list_lines) {
        std::vector<std::string> list_unapply_options = custom_split(line, " ");

        if (list_unapply_options[0] == SETTING_APPLYED)
            continue;
        list_unapply_options.erase(list_unapply_options.begin());
        for (std::string option : list_unapply_options) {
            bool    is_exist = false;

            for (std::string gen_option : general_list_unapply_options)
                if (gen_option == option) {
                    is_exist = true;
                    break ;
                }
            if (!is_exist)
                general_list_unapply_options.push_back(option);
        }
    }
    return general_list_unapply_options;
}


// Info part

std::map<std::string, std::string>	Parser::Info::pars_sysinfo(std::string str_sysinfo) {
	std::map<std::string, std::string> 	r_map;
	std::vector<std::string> list_line = Parser::custom_split(str_sysinfo, "\n");
	for (std::string line : list_line) {
		std::vector<std::string> segments_info = Parser::custom_split(line, ";");
		if (segments_info.size() == 2)
			r_map[segments_info[0]] = segments_info[1];
		else
			r_map[segments_info[0]] = "";
	}
	return r_map;
}

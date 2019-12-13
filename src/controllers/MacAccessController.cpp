#include "controllers/MacAccessController.hpp"


void 		MacAccessController::block_mac(std::string mac) {
	std::string 	script = Constant::ScriptExec::path_script + "ebt_block_mac.sh";

	this->_delete_rule_for_mac(mac);
	ScriptExecutor::execute(2, script.c_str(), mac.c_str());
}

void 		MacAccessController::add_mac_to_group(std::string mac, std::string group) {
	if (group == "Block") {
		this->block_mac(mac);
		return ;
	}
	std::map<std::string /*mac*/, t_accessLevel>	map_access_level = this->_access_controller.get_map_access_level();

	this->_delete_rule_for_mac(mac);
	for (auto item : map_access_level) {
		if (item.first == mac)
			continue;
		t_accessLevel	access_level = item.second;
		std::string 	check_mac = item.first;

		if (access_level.name_group != group)
			continue;
		std::string 	bind_script = Constant::ScriptExec::path_script + "bind_macs.sh " + mac + " " + check_mac;

		ScriptExecutor::execute(1, bind_script.c_str());
	}
}

void 		MacAccessController::_delete_rule_for_mac(std::string mac) {
	std::string			grep_script = Constant::ScriptExec::path_script + "get_ebt_rules_mac.sh";
	std::stringstream	ss_rules_mac;
	std::String 		line;

	ss_rules_mac(ScriptExecutor::getOutput::execute(2, grep_script.c_str(), mac.c_str()));
	while (getline(ss_rules_mac, line)) {
		std::string 	script_rm_rule = "ebtables -D " + line;

		ScriptExecutor::execute(1, script_rm_rule.c_str());
	}
}

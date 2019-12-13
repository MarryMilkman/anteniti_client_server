#include "controllers/AccessController.hpp"

#include "controllers/info_tools/ConnectedDeviceInfo.hpp"
#include "EventConnect.hpp"
#include "ScriptExecutor.hpp"

AccessController::AccessController() :
	_status_controller(StatusController::getInstance()),
	_cloud_controller(CloudController::getInstance()),
	_info_controller(RouterInfoController::getInstance())
{
	if (this->_init_tmp_map_from(Constant::Files::access_list))
		this->apply_tmp_map_access_level();
	if (this->_init_tmp_map_from(Constant::Files::cloud_access_list))
		this->apply_tmp_map_access_level();
}

AccessController::~AccessController() {}

AccessController &AccessController::getInstance() {
    static AccessController    _access_controller;

    return _access_controller;
}

///////////// /////////////// //////////////// //////////////// /////////////

// bool 			AccessController::apply_access_level_for_mac(std::string mac, std::string ip, bool is_conn) {
// 	t_accessLevel	&access_level = this->_map_access_level[mac];
// 	std::vector<std::string>	list_scripts_for_change_access = this->_form_scripts_access(mac, ip, is_conn, access_level);
//
// 	for (std::string script_for_change_access : list_scripts_for_change_access) {
// 		ScriptExecutor::execute(1, script_for_change_access.c_str());
// 	}
// 	// this->_map_access_level[mac] = access_level;
// 	// access_level.iface = iface;
// 	// this->_choose_pattern_and_execute(mac, access_level);
// 	// this->_map_access_level[mac] = access_level;
// }

// bool 			AccessController::apply_map_access_level() {
// 	std::vector<ConnectedDeviceInfo>	list_conn_dev = this->_info_controller.get_list_connected_devices();
//
// 	for (ConnectedDeviceInfo device : list_conn_dev) {
// 		if (this->_map_access_level.count(device._mac)) {
// 			bool 			is_conn = true;
// 			t_accessLevel	&access_level = this->_map_access_level[device._mac];
// 			std::vector<std::string>	list_scripts_for_change_access = this->_form_scripts_access(device._mac, device._ip, is_conn, access_level);
//
// 			for (std::string script_for_change_access : list_scripts_for_change_access) {
// 				ScriptExecutor::execute(1, script_for_change_access.c_str());
// 			}
// 			this->_map_access_level[device._mac] = this->_tmp_map_access_level[device._mac];
// 		}
// 	}
// 	return true;
// }

bool 			AccessController::apply_tmp_map_access_level() {
	if (!this->_tmp_map_access_level.size())
		return false;

	for (auto item : this->_tmp_map_access_level) {
		if (item.first.empty())
			continue;
		std::string					mac = item.first;
		std::string					old_name_group = item.second.old_name_group;
		std::string					name_group = item.second.name_group;
		if (old_name_group != name_group)
			this->_add_mac_to_group(mac, name_group, this->_tmp_map_access_level);
		// std::string					del_rule = this->_form_scripts_access(mac, old_name_group, 'D');
		// std::string					add_rule = this->_form_scripts_access(mac, name_group, 'A');

		// ScriptExecutor::execute(1, del_rule.c_str());
		// ScriptExecutor::execute(1, add_rule.c_str());
		this->_map_access_level[item.first] = item.second;
	}
	this->_rewrite_access_list();
	this->_tmp_map_access_level.clear();
	return true;
}

bool 			AccessController::refresh_tmp_map_access_level(std::string path_to_file) {
	std::cerr << "AccessController::refresh_tmp_map_access_level\n";
	this->_tmp_map_access_level.clear();
	this->_init_tmp_map_from(path_to_file);
	if (this->_tmp_map_access_level.size())
		return true;
	return false;
}

bool			AccessController::refresh_tmp_map_access_level(std::vector<EventConnect> list_events) {
	std::cerr << "AccessController::refresh_tmp_map_access_level\n";
	this->_tmp_map_access_level.clear();
	for (EventConnect event : list_events) {
		this->_tmp_map_access_level[event.mac] = t_accessLevel();
		this->_tmp_map_access_level[event.mac].iface = event.iface;
	}
	if (this->_tmp_map_access_level.size())
		return true;
	return false;
}

std::map<std::string /*name*/, Network>			&AccessController::get_map_networks() {
	return this->_map_networks;
}

std::map<std::string /*mac*/, t_accessLevel>	&AccessController::get_map_access_level() {
	return this->_map_access_level;
}



// MARK: - Private



bool 		AccessController::_init_tmp_map_from(std::string path_to_file) {
	std::fstream		file_accesslist;
	std::string 		line;
	std::string 		str_json;
	struct json_object	*f_js_array_access;

	if (path_to_file == Constant::Files::cloud_access_list)
		this->_cloud_controller.get_blocklist_from_cloud();
	file_accesslist.open(path_to_file);
	if (!file_accesslist.is_open())
		return false;
	this->_tmp_map_access_level.clear();
	while (getline(file_accesslist, line)) {
		str_json += line;
	}
	f_js_array_access = json_tokener_parse(str_json.c_str());
	if (!f_js_array_access || json_object_get_type(f_js_array_access) != json_type_array)
		return false;
	for (int i = 0, size = json_object_array_length(f_js_array_access); i < size; i++){
		struct json_object	*js_obj_access = json_object_array_get_idx(f_js_array_access, i);
		struct json_object	*js_obj_mac;
		// struct json_object	*js_obj_access_group;
		struct json_object	*js_obj_name;
		struct json_object	*js_obj_active_status;
		t_accessLevel 		access_level;
		std::string			mac;

		if (!js_obj_access)
			continue;

		js_obj_mac = json_object_object_get(js_obj_access, "AccessMAC");
		mac = json_object_get_string(js_obj_mac);
		// js_obj_access_group = json_object_object_get(js_obj_access, "access_group");
		js_obj_name = json_object_object_get(js_obj_access, "names");
		js_obj_active_status = json_object_object_get(js_obj_access, "ActivStatus");
		if (json_object_get_type(js_obj_mac) != json_type_string ||
				// json_object_get_type(js_obj_access_group) != json_type_string ||
				json_object_get_type(js_obj_name) != json_type_string ||
				json_object_get_type(js_obj_active_status) != json_type_string /*||
				json_object_get_string(js_obj_active_status) == "1"*/)
			continue;
		access_level.name_group = json_object_get_string(js_obj_name);
		access_level.need_deauth = json_object_get_string(js_obj_active_status) == "0";
		if (this->_map_access_level.count(mac))
			access_level.old_name_group = this->_map_access_level[mac].name_group;
		this->_tmp_map_access_level[mac] = access_level;
	}
	json_object_put(f_js_array_access);
	if (this->_tmp_map_access_level.size())
		return true;
	return false;
}

void 			AccessController::_rewrite_access_list() {
	struct json_object 	*f_js_array_access;

	f_js_array_access = json_object_new_array();
	for (auto item : this->_map_access_level) {
		std::string			mac = item.first;
		t_accessLevel		access_level = item.second;
		struct json_object	*new_js_obj = json_object_new_object();
		std::string			access_group = "-";
		//
		// access_group[0] = static_cast<char>(access_level.pattern);
		// access_group[1] = static_cast<char>(access_level.group) + '0';
		json_object_object_add(new_js_obj, "AccessMAC", json_object_new_string(mac.c_str()));
		json_object_object_add(new_js_obj, "access_group", json_object_new_string(access_group.c_str()));
		json_object_object_add(new_js_obj, "names", json_object_new_string(access_level.name_group.c_str()));
		json_object_object_add(new_js_obj, "ActivStatus", json_object_new_string("0"));
		json_object_array_add(f_js_array_access, new_js_obj);
	}
	std::ofstream	file(Constant::Files::access_list);

	file << json_object_get_string(f_js_array_access);
	json_object_put(f_js_array_access);
}



std::string		AccessController::_form_scripts_access(
											std::string mac,
											std::string name_group,
											char flag)
{
	if (mac.empty())
		return "";
	std::stringstream	ss_script("");

	ss_script << Constant::ScriptExec::script_path + "xer.sh";
	// for (size_t size = mac.size(), i = 0; i < size; i++)
	// 	mac[i] = std::toupper(mac[i]);
	if (name_group.empty())
		return "";
	ss_script << " " << mac << " " << this->_get_flag_group(name_group) << " " << flag;
	return ss_script.str();
}



bool 			AccessController::_is_mac_in_group(std::string mac, std::string group) {
	return false;
}


std::string 		AccessController::_get_flag_group(std::string name_group) {
	if (name_group == "Main")
		return "L";
	else if (name_group == "Guest")
		return "G";
	else if (name_group == "SmartHome")
		return "S";
	else if (name_group.empty())
		return "O";
	else if (name_group == "Blocked")
		return "B";
	else
		return "C";
}

void 		AccessController::_add_mac_to_group(std::string mac,
												std::string group,
												std::map<std::string /*mac*/, t_accessLevel> &map_access_level)
{
	if (group == "Block") {
		this->_block_mac(mac);
		return ;
	}
	int 	count = 0;

	this->_delete_rule_for_mac(mac);
	for (auto item : map_access_level) {
		if (item.first == mac)
			continue;
		t_accessLevel	access_level = item.second;
		std::string 	check_mac = item.first;

		if (access_level.name_group != group)
			continue;
		std::string 	bind_script = Constant::ScriptExec::script_path + "ebt_bind_macs.sh " + mac + " " + check_mac;

		ScriptExecutor::execute(1, bind_script.c_str());
		count++;
	}
	if (!count) {
		std::string 	bind_script = "ebtables -D FORWARD -d " + mac + " -j DROP;\n";

		bind_script = bind_script + "ebtables -A FORWARD -d " + mac + " -j DROP;\n";
		ScriptExecutor::execute(1, bind_script.c_str());
	}
}

void 		AccessController::_block_mac(std::string mac) {
	std::string 	script = Constant::ScriptExec::script_path + "ebt_block_mac.sh";

	this->_delete_rule_for_mac(mac);
	ScriptExecutor::execute(2, script.c_str(), mac.c_str());
}

void 		AccessController::_delete_rule_for_mac(std::string mac) {
	for (int i = 0, size = mac.size(); i < size ; i++)
		mac[i] = std::toupper(mac[i]);

	std::string			grep_script = Constant::ScriptExec::script_path + "ebt_get_rules_mac.sh";
	std::string 		line;
	std::stringstream	ss_rules_mac(ScriptExecutor::getOutput::execute(2, grep_script.c_str(), mac.c_str()));

	std::cerr << "HELLO!\n";
	std::cerr << ss_rules_mac.str() << "\n";
	while (getline(ss_rules_mac, line)) {
		std::string 	script_rm_rule = "ebtables -D " + line;

		ScriptExecutor::execute(1, script_rm_rule.c_str());
	}
}



// MARK : - t_accessLevel

s_accessLevel::s_accessLevel() {
	// this->pattern = al_Limited;
	// this->group = 0;
	this->name_group = "";
	this->old_name_group = "";
	this->iface = "wlan0";
	this->need_deauth = false;
}
s_accessLevel::~s_accessLevel() {

}

s_accessLevel	& s_accessLevel::operator=(s_accessLevel const & ref) {
	// this->pattern = ref.pattern;
	// this->list_group = ref.list_group;
	// this->group = ref.group;
	this->name_group = ref.name_group;
	this->old_name_group = ref.old_name_group;
	this->iface = ref.iface;
	this->need_deauth = ref.need_deauth;
	return *this;
}

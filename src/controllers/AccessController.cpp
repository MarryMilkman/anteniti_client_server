#include "controllers/AccessController.hpp"
#include "EventConnect.hpp"
#include "ScriptExecutor.hpp"

AccessController::AccessController() :
	_status_controller(StatusController::getInstance()),
	_cloud_controller(CloudController::getInstance())
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

bool 			AccessController::apply_access_level_for_mac(std::string mac, std::string iface) {
	t_accessLevel	access_level = this->_map_access_level[mac];

	access_level.iface = iface;
	this->_choose_pattern_and_execute(mac, access_level);
	this->_map_access_level[mac] = access_level;
}

bool 			AccessController::apply_map_access_level() {
	std::string 	mac;
	t_accessLevel 	access_level;

	std::cerr << "AccessController::apply_map_access_level\n";
	for (auto item : this->_map_access_level) {
		mac = item.first;
		access_level = item.second;
		this->_choose_pattern_and_execute(mac, access_level);
	}
	return true;
}

bool 			AccessController::apply_tmp_map_access_level() {
	std::string 	mac;
	t_accessLevel 	access_level;

	std::cerr << "AccessController::apply_tmp_map_access_level\n";
	if (!this->_tmp_map_access_level.size())
		return false;
	for (auto item : this->_tmp_map_access_level) {
		mac = item.first;
		if (!mac.size())
			continue;
		access_level = item.second;
		this->_choose_pattern_and_execute(mac, access_level);
		this->_map_access_level[mac] = access_level;
	}
	this->_rewrite_access_list();
	this->_tmp_map_access_level.clear();
	return true;
}

void 			AccessController::_choose_pattern_and_execute(std::string mac, t_accessLevel access_level) {
	if (!mac.size())
		return;
	if (access_level.name == "")
		this->_makeLimited(mac, access_level);
	// else if (access_level.name == eAccessPattern::al_Blocked)
	// 	this->_makeBlocked(mac, access_level);
	else if (access_level.name == "Guest")
		this->_makeGuest(mac, access_level);
	else if (access_level.name == "Main")
		this->_makeMain(mac, access_level);
	// else if (access_level.name == eAccessPattern::al_Smart)
	// 	this->_makeSmart(mac, access_level);
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
		struct json_object	*js_obj_access_group;
		struct json_object	*js_obj_name;
		struct json_object	*js_obj_active_status;
		t_accessLevel 		access_level;
		std::string 		str_access_group;

		if (!js_obj_access)
			continue;

		js_obj_mac = json_object_object_get(js_obj_access, "AccessMAC");
		js_obj_access_group = json_object_object_get(js_obj_access, "access_group");
		js_obj_name = json_object_object_get(js_obj_access, "names");
		js_obj_active_status = json_object_object_get(js_obj_access, "ActivStatus");
		if (json_object_get_type(js_obj_mac) != json_type_string ||
				json_object_get_type(js_obj_access_group) != json_type_string ||
				json_object_get_type(js_obj_name) != json_type_string ||
				json_object_get_type(js_obj_active_status) != json_type_string ||
				json_object_get_string(js_obj_active_status) == "1")
			continue;
		str_access_group = json_object_get_string(js_obj_access_group);
		if (!str_access_group.size())
			continue;
		access_level.pattern = static_cast<eAccessPattern>(str_access_group[0]);
		access_level.group = str_access_group.size() > 1 ? str_access_group[1] - '0' : 0;
		access_level.name = json_object_get_string(js_obj_name);
		this->_tmp_map_access_level[json_object_get_string(js_obj_mac)] = access_level;
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
		std::string			access_group = "..";

		access_group[0] = static_cast<char>(access_level.pattern);
		access_group[1] = static_cast<char>(access_level.group) + '0';
		json_object_object_add(new_js_obj, "AccessMAC", json_object_new_string(mac.c_str()));
		json_object_object_add(new_js_obj, "access_group", json_object_new_string(access_group.c_str()));
		json_object_object_add(new_js_obj, "names", json_object_new_string(access_level.name.c_str()));
		json_object_object_add(new_js_obj, "ActivStatus", json_object_new_string("0"));
		json_object_array_add(f_js_array_access, new_js_obj);
	}
	std::ofstream	file(Constant::Files::access_list);

	file << json_object_get_string(f_js_array_access);
	json_object_put(f_js_array_access);
}




// Makers:

void 		AccessController::_makeLimited(std::string mac, t_accessLevel access_level) {
	std::stringstream 					ss_script;

	std::map<std::string, std::string> 	info_map = get_dev_info_by_mac(mac);
	if (!mac.size())
		return;
	std::cerr << mac << " _makeLimited\n";
	// // del from lan
	// for (int i = 0, size = mac.size(); i < size; i++)
	// 	mac[i] = std::toupper(mac[i]);
	// ss_script << Constant::ScriptExec::script_path << "unblock.sh " << mac;
		// del from lan
	ss_script = std::stringstream("");
	ss_script << "/root/delfrom_lan.sh " << info_map["ip"];
	std::cerr << ss_script.str() << "\n";
	ScriptExecutor::execute(1, ss_script.str().c_str());
		// del from guest
	ss_script = std::stringstream("");
	ss_script << "/root/delfrom_guest.sh " << info_map["ip"];
	std::cerr << ss_script.str() << "\n";
	ScriptExecutor::execute(1, ss_script.str().c_str());
}

void 		AccessController::_makeBlocked(std::string mac, t_accessLevel access_level) {
	// std::stringstream ss_script;
	//
	// if (!mac.size())
	// 	return;
	std::cerr << mac << " _makeBlocked\n";
	// // del from lan
	// ss_script = std::stringstream("");
	// ss_script << Constant::ScriptExec::script_path << "delfrom_lan.sh " << mac;
	// ScriptExecutor::execute(1, ss_script.str().c_str());
	// // del from guest
	// ss_script = std::stringstream("");
	// ss_script << Constant::ScriptExec::script_path << "delfrom_guest.sh " << mac;
	// ScriptExecutor::execute(1, ss_script.str().c_str());
	// // add to sump (blacklist)
	// if (!this->_is_mac_from(mac, eNumWireless::nw_Sump)) {
	// 	ss_script = std::stringstream("");
	// 	ss_script << Constant::ScriptExec::script_path << "addto_sump.sh " << mac;
	// 	ScriptExecutor::execute(1, ss_script.str().c_str());
	// }
}

void 		AccessController::_makeGuest(std::string mac, t_accessLevel access_level) {
	std::stringstream 					ss_script;
	std::map<std::string, std::string> 	info_map = get_dev_info_by_mac(mac);

	if (!mac.size())
		return;
	std::cerr << mac << " _makeGuest\n";
	// // del from lan
	// for (int i = 0, size = mac.size(); i < size; i++)
	// 	mac[i] = std::toupper(mac[i]);
	// ss_script << Constant::ScriptExec::script_path << "unblock.sh " << mac;
		// del from lan
	ss_script = std::stringstream("");
	ss_script << "/root/delfrom_lan.sh " << info_map["ip"];
	std::cerr << ss_script.str() << "\n";
	ScriptExecutor::execute(1, ss_script.str().c_str());
		//add to guest
	ss_script = std::stringstream("");
	ss_script << "/root/addto_guest.sh " << info_map["ip"];
	std::cerr << ss_script.str() << "\n";
	ScriptExecutor::execute(1, ss_script.str().c_str());
		//rm from smart
}

void 		AccessController::_makeMain(std::string mac, t_accessLevel access_level) {
	std::stringstream 					ss_script;
	std::map<std::string, std::string> 	info_map = get_dev_info_by_mac(mac);

	if (!mac.size())
		return;
	std::cerr << mac << " _makeMain\n";
	// // del from lan
	// for (int i = 0, size = mac.size(); i < size; i++)
	// 	mac[i] = std::toupper(mac[i]);
	// ss_script << Constant::ScriptExec::script_path << "unblock.sh " << mac;
		// add to lan
	ss_script = std::stringstream("");
	ss_script << "/root/addto_lan.sh " << info_map["ip"];
	std::cerr << ss_script.str() << "\n";
	ScriptExecutor::execute(1, ss_script.str().c_str());
		//rm from guest
	ss_script = std::stringstream("");
	ss_script << "/root/delfrom_guest.sh " << info_map["ip"];
	std::cerr << ss_script.str() << "\n";
	ScriptExecutor::execute(1, ss_script.str().c_str());
		//rm from smart
}

void 		AccessController::_makeSmart(std::string mac, t_accessLevel access_level) {
	// std::stringstream ss_script;
	//
	// if (!mac.size())
	// 	return;
	std::cerr << mac << " _makeSmart\n";
	// // add to smart
	// if (!this->_is_mac_from(mac, eNumWireless::nw_Smurt)) {
	// 	ss_script = std::stringstream("");
	// 	ss_script << Constant::ScriptExec::script_path << "addto_smart.sh " << mac;
	// 	ScriptExecutor::execute(1, ss_script.str().c_str());
	// }
	// // add to sump (blacklist)
	// if (!this->_is_mac_from(mac, eNumWireless::nw_Sump)) {
	// 	ss_script = std::stringstream("");
	// 	ss_script << Constant::ScriptExec::script_path << "addto_sump.sh " << mac;
	// 	ScriptExecutor::execute(1, ss_script.str().c_str());
	// }
}

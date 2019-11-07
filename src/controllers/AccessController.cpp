#include "controllers/AccessController.hpp"
#include "EventConnect.hpp"
#include "ScriptExecutor.hpp"

AccessController::AccessController() :
	_status_controller(StatusController::getInstance()),
	_cloud_controller(CloudController::getInstance())
{
	if (this->_init_tmp_map_from(Constant::Files::path_access_list))
		this->apply_tmp_map_access_level();
	if (this->_init_tmp_map_from(Constant::Files::path_cloud_access_list))
		this->apply_tmp_map_access_level();
}

AccessController::~AccessController() {}

AccessController &AccessController::getInstance() {
    static AccessController    _access_controller;

    return _access_controller;
}

///////////// /////////////// //////////////// //////////////// /////////////


bool 			AccessController::apply_tmp_map_access_level() {
	std::string 	mac;
	eAccessLevel 	access_level;

	if (!this->_tmp_map_access_level.size())
		return false;
	for (auto item : this->_tmp_map_access_level) {
		mac = item.first;
		access_level = item.second;
		if (access_level == eAccessLevel::al_Limited)
			this->_makeLimited(mac);
		// else if (access_level == eAccessLevel::al_Blocked)
		// 	this->_makeBlocked(mac);
		// else if (access_level == eAccessLevel::al_Guest)
		// 	this->_makeGuest(mac);
		else if (access_level == eAccessLevel::al_General)
			this->_makeGeneral(mac);
		// else if (access_level == eAccessLevel::al_Smart)
		// 	this->_makeSmart(mac);
		this->_map_access_level[mac] = access_level;
	}
	this->_rewrite_access_list();
	this->_tmp_map_access_level.clear();
	return true;
}

bool 			AccessController::refresh_tmp_map_access_level(std::string path_to_file) {
	this->_tmp_map_access_level.clear();
	this->_init_tmp_map_from(path_to_file);
	if (this->_tmp_map_access_level.size())
		return true;
	return false;
}

bool			AccessController::refresh_tmp_map_access_level(std::vector<EventConnect> list_events) {
	this->_tmp_map_access_level.clear();
	for (EventConnect event : list_events)
		this->_tmp_map_access_level[event.mac] = eAccessLevel::al_Limited;
	if (this->_tmp_map_access_level.size())
		return true;
	return false;
}

std::map<std::string /*mac*/, eAccessLevel>	&AccessController::get_map_access_level() {
	return this->_map_access_level;
}



// MARK: - Private



bool 		AccessController::_init_tmp_map_from(std::string path_to_file) {
	std::fstream	file_accesslist;
	std::string 	line;

	if (path_to_file == Constant::Files::path_cloud_access_list)
		this->_cloud_controller.get_blocklist_from_cloud();
	file_accesslist.open(path_to_file);
	if (!file_accesslist.is_open())
		return false;
	this->_tmp_map_access_level.clear();
	while (getline(file_accesslist, line)) {
		std::stringstream 	ss(line);
		std::string 		mac;
		eAccessLevel 		access_level;
		std::string 		str_access_level;
		std::string 		active_status;

		ss >> mac;
		ss >> str_access_level;
		ss >> active_status;
		if (active_status == "1")
			continue;
		try {
			access_level = static_cast<eAccessLevel>(std::stoi(str_access_level));
			this->_tmp_map_access_level[mac] = access_level;
		} catch (std::exception &e) {}
	}
	if (this->_tmp_map_access_level.size())
		return true;
	return false;
}

void 			AccessController::_rewrite_access_list() {
	std::stringstream ss;

	for (auto item : this->_map_access_level)
		ss << item.first << " " << item.second << "\n";
	std::ofstream	file(Constant::Files::path_access_list);

	file << ss.str();
}




// Makers:

void 		AccessController::_makeLimited(std::string mac) {
	std::stringstream ss_script;

	if (!mac.size())
		return;
	std::cerr << mac << " _makeLimited\n";
	// // del from lan
	for (int i = 0, size = mac.size(); i < size; i++)
		mac[i] = std::toupper(mac[i]);
	ss_script << Constant::ScriptExec::script_path << "block.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
}

void 		AccessController::_makeBlocked(std::string mac) {
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

void 		AccessController::_makeGuest(std::string mac) {
	// std::stringstream ss_script;
	//
	// if (!mac.size())
	// 	return;
	std::cerr << mac << " _makeGuest\n";
	// // del from lan
	// ss_script = std::stringstream("");
	// ss_script << Constant::ScriptExec::script_path << "delfrom_lan.sh " << mac;
	// ScriptExecutor::execute(1, ss_script.str().c_str());
	// // add to guest
	// if (!this->_is_mac_from(mac, eNumWireless::nw_Guest)) {
	// 	ss_script = std::stringstream("");
	// 	ss_script << Constant::ScriptExec::script_path << "addto_guest.sh " << mac;
	// 	ScriptExecutor::execute(1, ss_script.str().c_str());
	// }
	// // add to sump (blacklist)
	// if (!this->_is_mac_from(mac, eNumWireless::nw_Sump)) {
	// 	ss_script = std::stringstream("");
	// 	ss_script << Constant::ScriptExec::script_path << "addto_sump.sh " << mac;
	// 	ScriptExecutor::execute(1, ss_script.str().c_str());
	// }
}

void 		AccessController::_makeGeneral(std::string mac) {
	std::stringstream ss_script;

	if (!mac.size())
		return;
	std::cerr << mac << " _makeLimited\n";
	// // del from lan
	for (int i = 0, size = mac.size(); i < size; i++)
		mac[i] = std::toupper(mac[i]);
	ss_script << Constant::ScriptExec::script_path << "unblock.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
}

void 		AccessController::_makeSmart(std::string mac) {
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

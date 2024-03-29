#include "controllers/BlockingController.hpp"
#include "ScriptExecutor.hpp"

BlockingController::BlockingController() :
	_status_controller(StatusController::getInstance()),
	_cloud_controller(CloudController::getInstance())

{
	std::string script = "mkdir " + Constant::Blocking::dit_blocklist;

	ScriptExecutor::getOutput::execute(1, script.c_str());
}

BlockingController::~BlockingController() {}

BlockingController &BlockingController::getInstance() {
    static BlockingController    setting;

    return setting;
}

// bool 		BlockingController::download_list() {
// 	std::fstream	file_blocklist(Constant::Blocking::path_blocklist);
// 	std::string 	line;
//
// 	this->_cloud_controller.get_blocklist_from_cloud();
// 	if (!file_blocklist.is_open())
// 		return false;
// 	this->_get_block_list_from_file();
// 	// while (getline(file_blocklist, line)) {
// 	// 	std::stringstream 	ss(line);
// 	// 	BlockDevice			block_d;
// 	// 	std::string 		status;
// 	//
// 	// 	ss >> block_d.mac;
// 	// 	ss >> status;
// 	// 	try {
// 	// 		block_d.access_level = static_cast<eAccessLevel>(std::stoi(status));
// 	// 		std::cerr << block_d.mac << " " << block_d.access_level << "\n";
// 	//
// 	// 		this->_tmp_block_list.push_back(block_d);
// 	// 	} catch (std::exception &e) {
// 	// 		block_d.access_level = eAccessLevel::al_Blocked;
// 	// 	}
// 	// }
// 	if (!this->_tmp_block_list.size())
// 		return false;
// 	return true;
// }


bool 		BlockingController::is_new_orders() {
	std::fstream	file_blocklist;

	file_blocklist.open(Constant::Blocking::path_blocklist);
	if (!file_blocklist.is_open())
		return false;
	this->_get_block_list_from_file();
	if (this->_tmp_block_list.size())
		return true;
	return false;
}



bool		BlockingController::apply() {
	std::cerr << "Macs from BlockingController::_execute:\n";
	std::fstream	file_blocklist;

	file_blocklist.open(Constant::Blocking::path_blocklist);
	if (!file_blocklist.is_open())
		return false;
	if (!this->_tmp_block_list.size())
		this->_get_block_list_from_file();
	for (BlockDevice block_d : this->_tmp_block_list) {
		if (block_d.access_level == eAccessLevel::al_Limited)
			this->_makeLimited(block_d.mac);
		else if (block_d.access_level == eAccessLevel::al_Blocked)
			this->_makeBlocked(block_d.mac);
		else if (block_d.access_level == eAccessLevel::al_Guest)
			this->_makeGuest(block_d.mac);
		else if (block_d.access_level == eAccessLevel::al_General)
			this->_makeGeneral(block_d.mac);
		else if (block_d.access_level == eAccessLevel::al_Smart)
			this->_makeSmart(block_d.mac);
	}
	if (this->_tmp_block_list.size())
		this->_status_controller.mac_list_reload();
	this->_tmp_block_list.clear();
	return true;
}

void 		BlockingController::_get_block_list_from_file() {
	std::fstream	file_blocklist(Constant::Blocking::path_blocklist);
	std::string 	line;

	if (!file_blocklist.is_open())
		return ;
	this->_tmp_block_list.clear();
	while (getline(file_blocklist, line)) {
		std::stringstream 	ss(line);
		BlockDevice			block_d;
		std::string 		status;
		std::string 		active_status;

		ss >> block_d.mac;
		ss >> status;
		ss >> active_status;
		if (active_status != "0")
			continue;
		try {
			block_d.access_level = static_cast<eAccessLevel>(std::stoi(status));
			std::cerr << block_d.mac << " " << block_d.access_level << "\n";

			this->_tmp_block_list.push_back(block_d);
		} catch (std::exception &e) {
			block_d.access_level = eAccessLevel::al_Blocked;
		}
	}
}


// Makers:

void 		BlockingController::_makeLimited(std::string mac) {
	std::stringstream ss_script;

	if (!mac.size())
		return;
	std::cerr << mac << " _makeLimited\n";
	// del from lan
	ss_script = std::stringstream("");
	ss_script << Constant::ScriptExec::script_path << "delfrom_lan.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
	// del from guest
	ss_script = std::stringstream("");
	ss_script << Constant::ScriptExec::script_path << "delfrom_guest.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
	// del from sump (blacklist)
	ss_script = std::stringstream("");
	ss_script << Constant::ScriptExec::script_path << "delfrom_sump.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
}

void 		BlockingController::_makeBlocked(std::string mac) {
	std::stringstream ss_script;

	if (!mac.size())
		return;
	std::cerr << mac << " _makeBlocked\n";
	// del from lan
	ss_script = std::stringstream("");
	ss_script << Constant::ScriptExec::script_path << "delfrom_lan.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
	// del from guest
	ss_script = std::stringstream("");
	ss_script << Constant::ScriptExec::script_path << "delfrom_guest.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
	// add to sump (blacklist)
	if (!this->_is_mac_from(mac, eNumWireless::nw_Sump)) {
		ss_script = std::stringstream("");
		ss_script << Constant::ScriptExec::script_path << "addto_sump.sh " << mac;
		ScriptExecutor::execute(1, ss_script.str().c_str());
	}
}

void 		BlockingController::_makeGuest(std::string mac) {
	std::stringstream ss_script;

	if (!mac.size())
		return;
	std::cerr << mac << " _makeGuest\n";
	// del from lan
	ss_script = std::stringstream("");
	ss_script << Constant::ScriptExec::script_path << "delfrom_lan.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
	// add to guest
	if (!this->_is_mac_from(mac, eNumWireless::nw_Guest)) {
		ss_script = std::stringstream("");
		ss_script << Constant::ScriptExec::script_path << "addto_guest.sh " << mac;
		ScriptExecutor::execute(1, ss_script.str().c_str());
	}
	// add to sump (blacklist)
	if (!this->_is_mac_from(mac, eNumWireless::nw_Sump)) {
		ss_script = std::stringstream("");
		ss_script << Constant::ScriptExec::script_path << "addto_sump.sh " << mac;
		ScriptExecutor::execute(1, ss_script.str().c_str());
	}
}

void 		BlockingController::_makeGeneral(std::string mac) {
	std::stringstream ss_script;

	if (!mac.size())
		return;
	std::cerr << mac << " _makeGeneral\n";
	// add to lan
	if (!this->_is_mac_from(mac, eNumWireless::nw_General)) {
		ss_script = std::stringstream("");
		ss_script << Constant::ScriptExec::script_path << "addto_lan.sh " << mac;
		ScriptExecutor::execute(1, ss_script.str().c_str());
	}
	// del from guest
	ss_script = std::stringstream("");
	ss_script << Constant::ScriptExec::script_path << "delfrom_guest.sh " << mac;
	ScriptExecutor::execute(1, ss_script.str().c_str());
	// add to sump (blacklist)
	if (!this->_is_mac_from(mac, eNumWireless::nw_Sump)) {
		ss_script = std::stringstream("");
		ss_script << Constant::ScriptExec::script_path << "addto_sump.sh " << mac;
		ScriptExecutor::execute(1, ss_script.str().c_str());
	}
}

void 		BlockingController::_makeSmart(std::string mac) {
	std::stringstream ss_script;

	if (!mac.size())
		return;
	std::cerr << mac << " _makeSmart\n";
	// add to smart
	if (!this->_is_mac_from(mac, eNumWireless::nw_Smurt)) {
		ss_script = std::stringstream("");
		ss_script << Constant::ScriptExec::script_path << "addto_smart.sh " << mac;
		ScriptExecutor::execute(1, ss_script.str().c_str());
	}
	// add to sump (blacklist)
	if (!this->_is_mac_from(mac, eNumWireless::nw_Sump)) {
		ss_script = std::stringstream("");
		ss_script << Constant::ScriptExec::script_path << "addto_sump.sh " << mac;
		ScriptExecutor::execute(1, ss_script.str().c_str());
	}
}

// void 		BlockingController::_wifi_relaod() {
// 	std::string 	script = Constant::ScriptExec::script_path + "hostapd_reload.sh";
//
// 	system(script.c_str());
// }





// if (block_d.access_level == eAccessLevel::al_Blocked) {
// 	//
// 	// script = Constant::ScriptExec::script_path + "delfromlan.sh;";
// 	// script += Constant::ScriptExec::script_path + + " blacklist.sh"
// }
// if (block_d.access_level == al_General || block_d.access_level == eAccessLevel::al_GuestGeneral) {
// 	if (this->_is_mac_from(block_d.mac, "lan"))
// 	continue;
// 	script = Constant::ScriptExec::script_path + "addtolan.sh";
// }
// // if (block_d.access_level == al_Guest || block_d.access_level == eAccessLevel::al_GuestGeneral) {
// 	// 	if (!this->_is_mac_from(block_d.mac, "guest"))
// 	// 		continue;
// 	// 	script = Constant::ScriptExec::script_path + "addtoguest.sh";
// 	// }
// 	if (script.size() && block_d.mac.size())
// 	ScriptExecutor::execute(2, script.c_str(), block_d.mac.c_str());




bool 		BlockingController::_is_mac_from(std::string mac, eNumWireless num_wireless) {
	std::string script;
	std::string result;

	script = std::string(Constant::ScriptExec::script_path + "is_mac_in_list.sh ") + mac + " " + std::to_string(num_wireless);
	result = ScriptExecutor::getOutput::execute(1, script.c_str());
	std::cerr << "RESULT: *" << result << "*\n";
	if (result == "1\n")
		return true;
	return false;
}









// struct BlockDevice

BlockDevice::BlockDevice() {
}

BlockDevice::~BlockDevice() {
}


BlockDevice::BlockDevice(BlockDevice const &ref) {
	*this = ref;
}

BlockDevice		&BlockDevice::operator=(BlockDevice const &ref) {
	this->mac = ref.mac;
	this->access_level = ref.access_level;
	return *this;
}

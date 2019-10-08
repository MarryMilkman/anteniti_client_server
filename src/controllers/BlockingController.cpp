#include "controllers/BlockingController.hpp"
#include "controllers/CloudController.hpp"
#include "ScriptExecutor.hpp"

BlockingController::BlockingController()
{
	ScriptExecutor::getOutput::execute(1, "mkdir " DIR_BLOCKLIST);
}

BlockingController::~BlockingController() {}

BlockingController &BlockingController::getInstance() {
    static BlockingController    setting;

    return setting;
}

bool 		BlockingController::download_list() {
	std::fstream	file_blocklist(PATH_BLOCKLIST);
	std::string 	line;

	CloudController::getInstance().get_blocklist_from_cloud();
	if (!file_blocklist.is_open())
		return false;
	while (getline(file_blocklist, line)) {
		std::stringstream 	ss(line);
		BlockDevice			block_d;
		std::string 		status;

		ss >> block_d.mac;
		ss >> status;
		try {
			block_d.access_level = static_cast<eAccessLevel>(std::stoi(status));
			std::cerr << block_d.mac << " " << block_d.access_level << "\n";

			this->_tmp_block_list.push_back(block_d);
		} catch (std::exception &e) {
			block_d.access_level = eAccessLevel::al_Blocked;
		}
	}
	if (!this->_tmp_block_list.size())
		return false;
	return true;
}

bool		BlockingController::apply() {
	std::cerr << "Macs from BlockingController::_execute:\n";
	std::fstream	file_blocklist(PATH_BLOCKLIST);
	std::string 	line;

	if (!file_blocklist.is_open())
		return false;

	for (BlockDevice block_d : this->_tmp_block_list) {
		std::string script = "";

		std::cerr << block_d.mac << " " << block_d.access_level << "\n";
		if (block_d.access_level == eAccessLevel::al_Blocked)
			script = SCRIPT_PATH "delfromlan.sh";
		if (block_d.access_level == al_General || block_d.access_level == eAccessLevel::al_GuestGeneral) {
			if (this->_is_mac_from(block_d.mac, "lan"))
				continue;
			script = SCRIPT_PATH "addtolan.sh";
		}
		// if (block_d.access_level == al_Guest || block_d.access_level == eAccessLevel::al_GuestGeneral) {
		// 	if (!this->_is_mac_from(block_d.mac, "guest"))
		// 		continue;
		// 	script = SCRIPT_PATH "addtoguest.sh";
		// }
		if (script.size() && block_d.mac.size())
			ScriptExecutor::execute(2, script.c_str(), block_d.mac.c_str());
	}
	// system("wifi reload");
	this->_tmp_block_list.clear();
	return true;
}

bool 		BlockingController::_is_mac_from(std::string mac, std::string name_network) {
	std::string script;
	std::string result;

	script = std::string(SCRIPT_PATH "is_mac_in_list_") + (name_network + ".sh");
	result = ScriptExecutor::getOutput::execute(2, script.c_str(), mac.c_str());
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

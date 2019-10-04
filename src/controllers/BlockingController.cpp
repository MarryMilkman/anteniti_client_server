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
			std::cerr << "hello\n";
			block_d.access_level = static_cast<eAccessLevel>(std::stoi(status));
			std::cerr << block_d.mac << " " << block_d.access_level << "\n";

			this->_tmp_block_list.push_back(block_d);
		} catch (std::exception &e) {
			block_d.access_level = eAccessLevel::al_Blocked;
		}
	}
	return true;
}

bool		BlockingController::apply() {
	std::cerr << "Macs from BlockingController::_execute:\n";
	std::fstream	file_blocklist(PATH_BLOCKLIST);
	std::string 	line;

	if (!file_blocklist.is_open())
		return false;
	// while (getline(file_blocklist, line)) {
	// 	std::stringstream 	ss(line);
	// 	BlockDevice			block_d;
	// 	std::string 		status;
	//
	// 	ss >> block_d.mac;
	// 	ss >> status;
	// 	try {
	// 		block_d.access_level = static_cast<eAccessLevel>(std::stoi(status));
	// 	} catch (std::exception &e) {
	// 		block_d.access_level = eAccessLevel::al_Blocked;
	// 	}
	// 	this->_tmp_block_list.push_back(block_d);
	// }

	for (BlockDevice block_d : this->_tmp_block_list) {
		std::string script;

		std::cerr << block_d.mac << " " << block_d.access_level << "\n";
		if (block_d.access_level == eAccessLevel::al_Blocked)
			script = SCRIPT_PATH "delfromlan.sh";
		if (block_d.access_level == al_General || block_d.access_level == eAccessLevel::al_GuestGeneral)
			script = SCRIPT_PATH "addtolan.sh";
		ScriptExecutor::execute(2, script.c_str(), block_d.mac.c_str());
	}
	// system("wifi reload");
	this->_tmp_block_list.clear();
	return true;
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

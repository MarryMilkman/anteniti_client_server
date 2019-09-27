#include "controllers/StatusController.hpp"
#include "ScriptExecutor.hpp"

#include "CustomException.hpp"

#include "Server.hpp"
#include "Client.hpp"

#include "Mutex.hpp"

StatusController::StatusController() {
    server_availabilit = false;
    this->_fresh();
}

StatusController::~StatusController() {

}

StatusController    &StatusController::getInstance() {
    std::lock_guard<std::mutex> guard(StatusController::_mutex);

    static StatusController controller;

    return controller;
}

// MARK : check is device master (have WAN)
bool        StatusController::isWAN() {
    std::lock_guard<std::mutex> guard(StatusController::_mutex);
	std::string 	script = SCRIPT_PATH "checkwan.sh";
    std::string     line;

	// return true;
	// return false;
    line = ScriptExecutor::getOutput::execute(1, script.c_str());
	std::cerr << "checkWAN " << line << "\n";
    if (line == "1\n")
        return true;
    return false;
}

    // MARK : choiceModWork
    // check router: master ? server : client
void        StatusController::choiceModWork() {
    this->_scan();
    if (this->_mod == eWorkMod::wm_server)
        this->_startServer();
    else if (this->_mod == eWorkMod::wm_client) {
        try {
            Client();
        } catch (CustomException &e) {
            std::cerr << e.what() << "\n";
            if (e.exceptType == e_need_server_work_mod)
                this->_startServer();
        }
    }
    this->_fresh();
}

eWorkMod    StatusController::getWorkMod() {
    return this->_mod;
}



void        StatusController::_fresh() {
    this->_mod = eWorkMod::wm_unknown;
}

void        StatusController::_scan() {
    if (StatusController::isWAN())
        this->_mod = eWorkMod::wm_server;
    else
        this->_mod = eWorkMod::wm_client;
}

void        StatusController::_startServer() {
    this->_mod = eWorkMod::wm_server;
    try {
        Server();
    } catch (CustomException &e) {
        std::cerr << e.what() << "\n";
    }
}

void        StatusController::_startClient() {
    this->_mod = eWorkMod::wm_client;
    try {
        Client();
    } catch (CustomException &e) {
        std::cerr << e.what() << "\n";
    }
}

std::mutex      StatusController::_mutex;

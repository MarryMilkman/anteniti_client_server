#include "controllers/StatusController.hpp"
#include "ScriptExecutor.hpp"

#include "CustomException.hpp"

#include "Server.hpp"
#include "Client.hpp"

StatusController::StatusController() {
    server_availabilit = false;
    this->_fresh();
}

StatusController::~StatusController() {

}

StatusController    &StatusController::getInstance() {
    static StatusController controller;

    return controller;
}

// MARK : check is device master (have WAN)
bool        StatusController::isWAN() {
    std::string     line;

    // return true;
    std::cerr << "checkWAN\n";
    // return true;
    line = ScriptExecutor::getOutput::checkWAN();
    if (line == "Online\n")
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

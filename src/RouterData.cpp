#include "RouterData.hpp"

RouterData::RouterData() {
}
RouterData::~RouterData(){}

RouterData::RouterData  (
                            std::string ip,
                            std::string login,
                            std::string pass,
                            std::string serial_number
                        )
{
    this->ip = ip;
	this->login = login;
	this->pass = pass;
	this->serial_number = serial_number;
    this->is_ok = true;
    this->message = "";
}

RouterData::RouterData(RouterData const & ref) {
    *this = ref;
}

RouterData &RouterData::operator=(RouterData const & ref) {
    this->ip = ref.ip;
	this->login = ref.login;
	this->pass = ref.pass;
	this->serial_number = ref.serial_number;
    this->is_ok = ref.is_ok;
    this->message = ref.message;
	return *this;
}

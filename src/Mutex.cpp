#include "Mutex.hpp"

// Mutex::Mutex() {
//     this->name = "noname\n";
// }
//
// Mutex::Mutex(std::string name) {
//     this->name = name;
// }
//
// Mutex::~Mutex() {
// }


// MARK: - Lock

Lock::Lock(std::mutex &mutex, std::string name) : _mutex(mutex), _name(name) {
    this->_lock();
}

Lock::~Lock() {
    this->_unlock();
}

void        Lock::_lock() {
    std::string str =  "try get lock " + this->_name + "\n";

    // std::cerr << str.c_str();
    this->_mutex.lock();

    // str = "lock mutex: " + this->_name + "\n";
    // std::cerr << str.c_str();
}

void        Lock::_unlock() {
    this->_mutex.unlock();
    std::string str;

    str += "unlock mutex: " + this->_name + "\n";
    // std::cerr << str.c_str();
}

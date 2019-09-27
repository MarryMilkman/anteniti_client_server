#ifndef ROUTER_DATA_HPP
#define ROUTER_DATA_HPP

#include <iostream>

class RouterData
{
public:
    RouterData();
    RouterData(
            std::string ip,
            std::string login,
            std::string pass,
            std::string serial_number);
    RouterData(RouterData const & ref);
    ~RouterData();
    RouterData &operator=(RouterData const &ref);

    std::string     ip;
    std::string     login;
    std::string     pass;
    std::string     serial_number;

    bool            is_ok;
    std::string     message;
};

#endif

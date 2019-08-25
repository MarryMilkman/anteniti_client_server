#ifndef CUSTOM_EXCEPTION
# define CUSTOM_EXCEPTION

#include "lib.h"

enum eExceptType
{
    e_ok = 0,
    e_need_server_work_mod,
    e_need_client_work_mod,
    e_need_any_work_mod,
    e_WAN_wasChange,

};

class CustomException: public std::exception
{
    CustomException() throw();
public:
    ~CustomException() throw();
    CustomException(eExceptType exceptType) throw();
    CustomException(CustomException const & ref) throw();
    CustomException &operator=(CustomException const & ref) throw();

    virtual const char      *what() const throw();
    eExceptType             exceptType;
};



#endif

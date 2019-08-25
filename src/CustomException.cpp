#include "CustomException.hpp"

CustomException::CustomException() throw()
{
}

CustomException::CustomException(CustomException const & ref) throw()
{
	*this = ref;
}

CustomException::CustomException(eExceptType errorType) throw()
{
    this->exceptType = errorType;
}

CustomException::~CustomException() throw()
{
}

const char*	CustomException::what() const throw()
{
    eExceptType   te = this->exceptType;

    if (te == eExceptType::e_ok)
        return "ok";
    if (te == eExceptType::e_need_server_work_mod)
        return "e_need_server_work_mod";
    if (te == eExceptType::e_need_client_work_mod)
        return "e_need_client_work_mod";
    if (te == eExceptType::e_need_any_work_mod)
        return "e_need_any_work_mod";
    if (te == eExceptType::e_WAN_wasChange)
        return "e_WAN_wasChange";
    return "Some error";
}

CustomException	&CustomException::operator=(CustomException const & ref) throw ()
{
	this->exceptType = ref.exceptType;
	return *this;
}

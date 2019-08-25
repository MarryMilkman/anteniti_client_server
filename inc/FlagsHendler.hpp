#ifndef FLAGS_HENDLER_HPP
#define FLAGS_HENDLER_HPP

#include "lib.h"

class FlagsHendler
{
	FlagsHendler();
public:
	FlagsHendler(int const argc, char const **argv, std::string &host, int &port, int &mod);
	~FlagsHendler();
	
private:
	std::string		_host;
	int				_port;
	int				_mod;

	void			_pars_and_init(int const argc, char const **argv);
};
#endif
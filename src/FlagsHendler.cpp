#include "FlagsHendler.hpp"

FlagsHendler::FlagsHendler(){}

FlagsHendler::~FlagsHendler(){}

FlagsHendler::FlagsHendler(int const argc, char const **argv, std::string &host, int &port, int &mod) {
	this->_host = "0.0.0.0";
	this->_port = 9897;
	this->_mod = -1;
	if (argc == 1)
		this->_mod = 2;
	else
		this->_pars_and_init(argc, argv);
	// std::vector<std::string> params;

	// for (int i = 1; i < argc; i++)
	// 	params.push_back(argv[i]);

	host = this->_host;
	port = this->_port;
	mod = this->_mod;
}

void			FlagsHendler::_pars_and_init(int const argc, char const **argv) {
	std::vector<std::string>	params;

	for (int i = 1; i < argc; i++)
		params.push_back(argv[i]);
	int		i = -1;
	int		size = params.size();

	while (++i < size) {
		if (params[i] == "-s" || params[i] == "-c") {
			if (this->_mod >= 0) {
				std::cerr << "Error: only one flag -s or -c\n";
				exit (0);
			}
			if (params[i] == "-s")
				this->_mod = 0;
			else
				this->_mod = 1;
		}
		else if (params[i] == "-h") {
			if (i + 1 == size) {
				std::cerr << "Error: host not specified (flag -h)\n";
				exit(0);
			}
			this->_host = params[i + 1];
			i++;
		}
		else if (params[i] == "-p") {
			if (i + 1 == size) {
				std::cerr << "Error: host not specified (flag -h)\n";
				exit(0);
			}
			try {
				this->_port = std::stoi(params[i + 1]);
			}
			catch (std::exception & e) {
				std::cerr << "Error: port must be a number\n";
				exit(0);
			}
			i++;
		}
		else {
			std::cerr << "Error: unexpected flag: " << params[i] << "\nValid flags:\n"
						<< "-h (specified host)\n" << "-p (specified port)\n"
						<< "-s (server mod)\n" << "-c (client mod)\n";
			exit(0);
		}
	}
	if (this->_mod < 0)
		this->_mod = 2;
}

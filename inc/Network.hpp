#ifndef NETWORK_HPP
# define NETWORK_HPP

# include "lib.h"

class Network {
public:
	Network() {}
	Network(std::string name);
	Network(json_object *obj);
	Network(Network const & ref);
	~Network();
	Network &operator=(Network const &ref);

	std::string	name;
};

#endif

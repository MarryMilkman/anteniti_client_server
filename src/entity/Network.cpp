#include "Network.hpp"


Network::Network(std::string json_str) {
	json_object 	*f_general_obj;
	json_object		*obj;

	f_general_obj = json_tokener_parse(json_str.c_str());
	this->name = json_object_get_string(json_object_object_get(f_general_obj, "names"));
	json_object_put(f_general_obj);
}

Network::Network(json_object *obj) {
	this->name = json_object_get_string(json_object_object_get(obj, "names"));
}

Network::Network(Network const & ref) {
	*this = ref;
}

Network::~Network() {

}

Network &Network::operator=(Network const &ref) {
	this->name = ref.name;
	return *this;
}

#ifndef EVENT_CONNECT_HPP
#define EVENT_CONNECT_HPP

#include "lib.h"

class EventConnect {
public:
	EventConnect();
	EventConnect(std::string str_event);
	~EventConnect();
	EventConnect(EventConnect const & ref);
	EventConnect	&operator=(EventConnect const & ref);

	bool 		conn;
	bool 		is_new;
	std::string nick;
	std::string	mac;
	std::string iface;
	time_t 		event_time;

	std::string get_str() const;
};



#endif

#ifndef EVENT_HPP
#define EVENT_HPP

#include "lib.h"

class Event {
public:
	Event();
	Event(std::string str_event);
	~Event();
	Event(Event const & ref);
	Event	&operator=(Event const & ref);

	bool 		conn;
	bool 		is_new;
	std::string nick;
	std::string	mac;
	std::string iface;
	time_t 		event_time;

	std::string get_str() const;
};



#endif

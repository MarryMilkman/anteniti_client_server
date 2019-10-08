#ifndef TIMER_HPP
# define TIMER_HPP

# include "lib.h"

class Timer
{
public:
    Timer();
    ~Timer();

    bool        one_time_in(int period);

private:
    bool        _status;
    time_t      _t;
	time_t 		_last_activity;
};

#endif

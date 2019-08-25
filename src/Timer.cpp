#include "Timer.hpp"

Timer::Timer() {
    this->_status = true;
}
Timer::~Timer() {}

bool 	Timer::one_time_in(int period) {
	time(&this->_t);
	if (!(this->_t % period) && this->_status) {
		this->_status = false;
		return true;
	}
	if (!this->_status && (this->_t % period))
		this->_status = true;
	return false;

}

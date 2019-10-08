#include "Timer.hpp"

Timer::Timer() {
    this->_status = true;
	this->_last_activity = 0;
}
Timer::~Timer() {}

bool 	Timer::one_time_in(int period) {
	time(&this->_t);
	if ((this->_t - this->_last_activity) >= period) {
		time(&this->_last_activity);
		return true;
	}
	// if (!(this->_t % period) && this->_status) {
	// 	this->_status = false;
	// 	time(&this->_last_activity);
	// 	return true;
	// }
	// if (!this->_status && (this->_t % period))
	// 	this->_status = true;
	return false;

}

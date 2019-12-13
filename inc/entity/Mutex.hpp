#ifndef MUTEX_HPP
# define MUTEX_HPP

# include "lib.h"
//
// class Mutex {
// public:
//     Mutex(std::string name);
//     Mutex();
//     ~Mutex();
//
//     std::mutex  mutex;
//     std::string name;
// };

class Lock {
public:
    Lock(std::mutex &mutex, std::string name);
    Lock() = delete;
    ~Lock();

private:
    std::mutex  &_mutex;
    std::string _name;

    void        _lock();
    void        _unlock();
};

#endif

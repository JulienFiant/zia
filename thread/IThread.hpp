#ifndef ITHREAD_HPP
#define ITHREAD_HPP

#include <mutex>
#include <thread>
#include <atomic>

class IThread
{
public:
    virtual ~IThread(){};
    virtual void lockThread(){};
    virtual void unlockThread(){};
    virtual bool isAlive() = 0;
    virtual void waitEnd(){};
    virtual void run(){};
};

#endif
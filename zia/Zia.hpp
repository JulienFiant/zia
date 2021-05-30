#ifndef ZIA_HPP
#define ZIA_HPP

#include "../config/ConfigManager.hpp"
#include "../config/ModuleManager.hpp"
#include "../network/Server.hpp"

class Zia
{
public:
    Zia();
    ~Zia();
    int Core();
    void start_thread();

protected:
    ConfigManager *m;
    ModuleManager *mm;
    Server *serv;
    std::atomic_bool run;
    std::thread tmp;
};

#endif
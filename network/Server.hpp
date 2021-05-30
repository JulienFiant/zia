#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Socket.hpp"
#include <mutex>
#include <thread>
#include <atomic>
#include "../bref/Pipeline.h"

class Server
{
public:
    Server();
    Server(const int &port);
    ~Server();
    void setPort(const int &port);
    void startServer();
    void acceptCli();
    void setModule(const std::list<Module *> &module_list);
    void start_thread(const int &tmp_cli);
    Module *getPhp();

protected:
    //Socket winsock;
    sockaddr_in serv;
    int state_serv;
    int port;
    int sock;
    std::atomic_bool run;
    std::list<std::thread *> list_tmp;
    std::list<Module *> module_list;
    std::mutex mutex;
    bref::Pipeline *pipeline;
};

#endif
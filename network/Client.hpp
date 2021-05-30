#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <iostream>
#include <list>
#include <ctime>
#include <vector>
#include <filesystem>
#include <fstream>
#include "Socket.hpp"
#include "../config/Module.hpp"
#include "IClient.hpp"
#include "../bref/HttpRequest.h"
#include "../bref/HttpResponse.h"

class Client : public IClient
{
public:
    Client(const int &sock);
    ~Client();
    void setPhp(Module *php);
    void SendHttp(const std::string &msg);
    std::string Receive();
    void ParseRequest();
    void GetMethod();
    void PostMethod();
    std::vector<std::string> split(const std::string &s, const std::string &delimiter);

protected:
    //Socket winsock;
    int sock;
    std::string req;
    bref::HttpRequest *httpreq;
    bref::HttpResponse *httpres;
    Module *php;
};

#endif
#ifndef ICLIENT_HPP
#define ICLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <string>

class IClient
{
public:
    virtual ~IClient(){};
    virtual void SendHttp(const std::string &msg) = 0;
    virtual std::string Receive() = 0;
    virtual void ParseRequest() = 0;
    virtual void GetMethod() = 0;
    virtual void PostMethod() = 0;
};

#endif
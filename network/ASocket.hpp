/*
** EPITECH PROJECT, 2020
** CPP_zia_2019 [WSL: Debian]
** File description:
** Socket
*/

#ifndef ASOCKET_HPP_
#define ASOCKET_HPP_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

class ASocket
{
public:
    ~ASocket(){};
    virtual void CreateSock() = 0;
    virtual void DeleteSock(int cock) = 0;

protected:
private:
};

#endif /* !SOCKET_HPP_ */

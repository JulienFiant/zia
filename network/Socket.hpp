/*
** EPITECH PROJECT, 2020
** CPP_zia_2019 [WSL: Debian]
** File description:
** Socket
*/

#ifndef SOCKET_HPP_
#define SOCKET_HPP_

#ifdef _WIN32
#include <winsock2.h>
#endif

#include "ASocket.hpp"

class Socket : public ASocket
{
public:
    Socket(){};
    ~Socket(){};
    void CreateSock()
    {
#ifdef __linux__
#elif _WIN32
        WSAStartup(MAKEWORD(2, 0), &WSAData);
#else
#endif
    };
    void DeleteSock(int cock)
    {
#ifdef __linux__
        close(cock);
#elif _WIN32
        WSACleanup();
        closesocket(cock);
#else
#endif
    }

protected:
private:
#ifdef _WIN32
    WSADATA WSAData;
#endif
};

#endif /* !SOCKET_HPP_ */

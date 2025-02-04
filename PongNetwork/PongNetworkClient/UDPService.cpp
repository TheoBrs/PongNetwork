#include <ws2tcpip.h>
#include <iostream>
#include "UDPService.h"

int UDPService::InitWinsock()
{
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
    {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }
    return 0;
}

int UDPService::InitSocket()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "Erreur socket !" << std::endl;
        WSACleanup();
        return 1;
    }
    return 0;
}

int UDPService::Init()
{
    InitWinsock();
    InitSocket();
    return 0;
}




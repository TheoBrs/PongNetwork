#include <ws2tcpip.h>
#include <iostream>
#include "UDPService.h"

void UDPService::InitWinsock()
{
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
    {
        std::cerr << "Erreur Winsock !" << std::endl;
        return;
    }

}

void UDPService::Init()
{
    InitWinsock();
}

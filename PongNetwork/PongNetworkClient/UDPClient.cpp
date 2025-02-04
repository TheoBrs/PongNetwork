#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "UDPClient.h"

int UDPClient::Init()
{
    UDPService::Init();
    return 0;
}

void UDPClient::SendMessage(std::string message)
{
    UDPService::SendMessage(message);
}

unsigned long long UDPClient::ReceiveMessage()
{
    return 0;
}

int UDPClient::TryConnect(const std::string& ip, unsigned port)
{
    const char* cstr = ip.c_str(); 
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, cstr, &m_serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Erreur de conversion d'adresse IP !" << std::endl;
        return 1;
    }

    std::cout << "Tentative de connexion au serveur..." << std::endl;

    std::string message = std::to_string(0) + " " + std::to_string(0);
    if (!sendto(m_socket, message.c_str(), message.size(), 0, (sockaddr*)&m_serverAddr,
        sizeof(m_serverAddr)))
    {
        std::cerr << "Impossible d'envoyer un message au serveur !" << std::endl;
        return 1;
    }

    std::cout << "Demande de connexion envoyee au serveur." << std::endl;
    return 0;
}
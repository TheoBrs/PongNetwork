#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "UDPClient.h"

int UDPClient::Init()
{
    UDPService::Init();
    
    u_long mode = 1;  
    ioctlsocket(m_socket, FIONBIO, &mode);

    return 0;
}

void UDPClient::SendMessagesUDP()
{
    UDPService::SendMessagesUDP();
    for (auto message : m_messages) 
    {
        if (!sendto(m_socket, message.c_str(), message.size(), 0, (sockaddr*)&m_serverAddr, sizeof(m_serverAddr)))
        {
            std::cerr << "Impossible d'envoyer un message au serveur !" << std::endl;
            continue;
        }
    }
    m_messages.clear();
}

void UDPClient::AddMessageToSend(std::string message)
{
    m_messages.push_back(message);
}

int UDPClient::ReceiveMessage(char (&buffer)[BUFFER_SIZE])
{
    int serverAddrSize = sizeof(m_serverAddr);
    int bytesReceived = recvfrom(m_socket, buffer, BUFFER_SIZE, 0, (sockaddr*)&m_serverAddr, &serverAddrSize);

    if (bytesReceived == SOCKET_ERROR)
    {
        //wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
    }
    else
    {
        buffer[bytesReceived] = '\0';
    }
    
    return bytesReceived;
}

int UDPClient::TryConnect(const std::string& ip, unsigned port, std::string name)
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

    std::string message = "ConnectionRequest " + name;
    AddMessageToSend(message);

    std::cout << "Demande de connexion envoyee au serveur." << std::endl;
    return 0;
}
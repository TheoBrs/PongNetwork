#include <ws2tcpip.h>
#include <iostream>
#include <array>
#include <SFML/System/Time.hpp>
#include "UDPServer.h"
int UDPServer::InitWinsock()
{
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
    {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }
    return 0;
}

int UDPServer::InitSocket()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket == INVALID_SOCKET) {
        std::cerr << "Erreur socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    // Server adress and port configuration
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(PORT);

    // Associate socket to adress and port
    if (bind(m_socket, (struct sockaddr*)&m_address, sizeof(m_address)) == SOCKET_ERROR)
    {
        std::cerr << "Erreur bind !" << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return 1;
    }

    u_long mode = 1;
    ioctlsocket(m_socket, FIONBIO, &mode);

}

int UDPServer::Init()
{
    int error;
    error = InitWinsock();
    if (error != 0) 
    {
        return error;
    }

    error = InitSocket();
    if (error != 0)
    {
        return error;
    }
    return 0;
}

void UDPServer::SendDirectMessageToAddr(sockaddr_in addr, std::string message)
{
    sendto(m_socket, message.c_str(), message.size(), 0, (sockaddr*)&addr, sizeof(addr));
}



void UDPServer::AddMessageUDP(int clientID, const std::string& message)
{
    m_messagesToSend[clientID].push_back(message);
}

void UDPServer::AddMessageUDPAll(const std::string& message)
{
    for (auto [id, _] : m_clients) 
    {
        AddMessageUDP(id, message);
    }
}

void UDPServer::SendMessages()
{
    for (auto& [clientId, messageList] : m_messagesToSend) 
    {
        for (auto message : messageList) 
        {
            sendto(
                m_socket,
                message.c_str(),
                message.size(),
                0,
                (sockaddr*)&m_clients[clientId].Address, sizeof(m_clients[clientId].Address));
        }
        m_messagesToSend[clientId].clear();
    }
}

int UDPServer::ReceiveMessages(BuffersToTreat* buffers)
{
    int bytesReceived;
    char buffer[BUFFER_SIZE];
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    do
    {
        bytesReceived = recvfrom(m_socket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived != SOCKET_ERROR)
        {
            buffer[bytesReceived] = '\0';
            std::array<char, BUFFER_SIZE> bufferStd;
            std::copy(std::begin(buffer), std::end(buffer), std::begin(bufferStd));
            (*buffers).push_back({ clientAddr, bufferStd });
        }

    } while (bytesReceived != SOCKET_ERROR);

    return 0;
}


void UDPServer::AddClient(int clientID, const sockaddr_in& address, const std::string& name)
{
    m_clients[clientID] = {name, address, new sf::Clock(), true};
    m_messagesToSend[clientID] = std::vector<std::string>();

}

void UDPServer::SetClientConnected(int id, bool value)
{
    m_clients[id].IsConnected = value;
}

bool UDPServer::GetIsClientConnected(int id)
{
    return m_clients[id].IsConnected;
}

sf::Time UDPServer::GetClientElapsedTimeLastPing(int id)
{
    return m_clients[id].PingTime->getElapsedTime();
}

void UDPServer::RestartPingTimeClient(int id)
{
    m_clients[id].PingTime->restart();
}

std::string UDPServer::GetClientName(int id)
{
    return m_clients[id].Name;
}

std::unordered_map<int, Client> UDPServer::GetClients()
{
    return m_clients;
}

int UDPServer::GetFirstPlayerDisconnected()
{
    for (auto [id, client] : m_clients) 
    {
        if (!client.IsConnected)
        {
            return id;
        }
    }

    return -1;
}

void UDPServer::UnInit()
{
    closesocket(m_socket);
    WSACleanup();
}



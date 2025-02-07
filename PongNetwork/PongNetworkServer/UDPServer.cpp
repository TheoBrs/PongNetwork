#include <ws2tcpip.h>
#include <iostream>
#include <array>
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
    return 0;

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
            (*buffers)[clientAddr].push_back(bufferStd);
        }

    } while (bytesReceived != SOCKET_ERROR);

    return 0;
}

void UDPServer::AddClient(int clientID, const sockaddr_in& address, const std::string& name)
{
    m_clients[clientID] = {name, address };
    m_messagesToSend[clientID] = std::vector<std::string>();
}

std::unordered_map<int, Client> UDPServer::GetClients()
{
    return m_clients;
}

void UDPServer::UnInit()
{
    closesocket(m_socket);
    WSACleanup();
}



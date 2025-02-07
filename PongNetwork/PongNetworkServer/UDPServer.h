#pragma once
#include <string>
#include <WinSock2.h>
#include <unordered_map>
#include "ServerSettings.h"


typedef  std::vector<std::pair<sockaddr_in, std::array<char, BUFFER_SIZE>>> BuffersToTreat;

struct Client 
{
    std::string Name;
    sockaddr_in Address;
};

class UDPServer
{
private:
    int InitWinsock();
    int InitSocket();

protected:
    char m_buffer[BUFFER_SIZE];
    WSADATA m_wsaData;
    SOCKET m_socket;
    sockaddr_in m_address;
    std::unordered_map<int, Client> m_clients;
    std::unordered_map<int, std::vector<std::string>> m_messagesToSend;
    
public:
    int Init();
    void SendDirectMessageToAddr(sockaddr_in addr, std::string message);
    void AddMessageUDP(int clientID, const std::string& message);
    void AddMessageUDPAll(const std::string& message);
    void SendMessages();
    int ReceiveMessages(BuffersToTreat* buffers);


    void AddClient(int clientID, const sockaddr_in& address, const std::string& name);
    std::unordered_map<int, Client> GetClients();

    void UnInit();
};


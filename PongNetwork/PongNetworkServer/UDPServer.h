#pragma once
#include <string>
#include <WinSock2.h>
#include <unordered_map>
#include "ServerSettings.h"

struct SockAddrHash {
    size_t operator()(const sockaddr_in& addr) const {
        // Combinaison simple des membres `sin_addr.s_addr` et `sin_port` pour créer un hash
        return std::hash<uint32_t>()(addr.sin_addr.s_addr) ^ std::hash<uint16_t>()(addr.sin_port);
    }
};

struct SockAddrEqual {
    bool operator()(const sockaddr_in& lhs, const sockaddr_in& rhs) const {
        return lhs.sin_addr.s_addr == rhs.sin_addr.s_addr && lhs.sin_port == rhs.sin_port;
    }
};

typedef  std::unordered_map<sockaddr_in, std::vector<std::array<char, BUFFER_SIZE>>, SockAddrHash, SockAddrEqual> BuffersToTreat;

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
    void AddMessageUDP(int clientID, const std::string& message);
    void AddMessageUDPAll(const std::string& message);
    void SendMessages();
    int ReceiveMessages(BuffersToTreat* buffers);

    void AddClient(int clientID, const sockaddr_in& address, const std::string& name);
    std::unordered_map<int, Client> GetClients();

    void UnInit();
};


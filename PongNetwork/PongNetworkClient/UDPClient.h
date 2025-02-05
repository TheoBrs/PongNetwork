#pragma once
#include "UDPService.h"

class UDPClient: public UDPService
{
private:
    sockaddr_in m_serverAddr;
    
public:
    int Init() override;
    void SendMessageUDP(std::string message) override;
    int ReceiveMessage(char (&buffer)[BUFFER_SIZE]) override;
    int TryConnect(const std::string& ip, unsigned int port, std::string name);

};

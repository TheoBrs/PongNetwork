#pragma once
#include "UDPService.h"

class UDPClient: public UDPService
{
private:
    sockaddr_in m_serverAddr;
    
public:
    int Init() override;
    void SendMessage(std::string message) override;
    unsigned long long ReceiveMessage() override;
    int TryConnect(const std::string& ip, unsigned int port);

};

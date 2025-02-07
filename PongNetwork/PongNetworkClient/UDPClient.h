#pragma once
#include <vector>
#include <string>
#include "UDPService.h"

class UDPClient: public UDPService
{
private:
    sockaddr_in m_serverAddr;
    std::vector<std::string> m_messages;
public:
    int Init() override;
    void SendMessagesUDP() override;
    void AddMessageToSend(std::string message);
    int ReceiveMessage(char (&buffer)[BUFFER_SIZE]) override;
    int TryConnect(const std::string& ip, unsigned int port, std::string name);

};

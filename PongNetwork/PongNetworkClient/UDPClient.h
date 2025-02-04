#pragma once
#include "UDPService.h"

class UDPClient: public UDPService
{
private:
    
    
public:
    void Init() override;
    void SendMessage(std::string message) override;
    unsigned long long ReceiveMessage() override;
};

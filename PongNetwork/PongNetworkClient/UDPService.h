#pragma once
#include <string>
#include <WinSock2.h>

class UDPService
{
private:
    void InitWinsock();
protected:
    WSADATA m_wsaData;
public:
    virtual void Init();
    virtual void SendMessage(std::string message){};
    virtual unsigned long long ReceiveMessage(){};
    
};

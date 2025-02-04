#pragma once
#include <string>
#include <WinSock2.h>

class UDPService
{
private:
    int InitWinsock();
    int InitSocket();
protected:
    WSADATA m_wsaData;
    SOCKET m_socket;
public:
    virtual int Init();
    virtual void SendMessage(std::string message){};
    virtual unsigned long long ReceiveMessage() = 0;
    void UnInit();
};

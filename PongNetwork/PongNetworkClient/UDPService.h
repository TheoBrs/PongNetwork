#pragma once
#include <string>
#include <WinSock2.h>

#define BUFFER_SIZE 1024

class UDPService
{
private:
    int InitWinsock();
    int InitSocket();
protected:
    char m_buffer[BUFFER_SIZE];
    WSADATA m_wsaData;
    SOCKET m_socket;
public:
    virtual int Init();
    virtual void SendMessagesUDP(){}
    virtual int ReceiveMessage(char (&buffer)[BUFFER_SIZE])  = 0;
    void UnInit();
};

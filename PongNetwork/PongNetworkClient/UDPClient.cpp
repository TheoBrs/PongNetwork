#include <WinSock2.h>
#include "UDPClient.h"

void UDPClient::Init()
{
    UDPService::Init();
}

void UDPClient::SendMessage(std::string message)
{
    UDPService::SendMessage(message);
}

unsigned long long UDPClient::ReceiveMessage()
{
    return UDPService::ReceiveMessage();
}

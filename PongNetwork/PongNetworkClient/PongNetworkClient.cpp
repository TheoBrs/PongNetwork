#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SFML/Graphics.hpp>

#include "Ball.h"
#include "Client.h"
#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 54000
#define BUFFER_SIZE 1024

sf::Vector2f screenSize;

int main()
{
#pragma region NewSFMLWindow
    
    Client client = Client();
    client.Run();

#pragma endregion

    return 0;
}
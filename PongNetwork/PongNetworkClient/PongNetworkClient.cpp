#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SFML/Graphics.hpp>

#include "Paddle.h"
#include "App.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 54000
#define BUFFER_SIZE 1024

sf::Vector2f screenSize;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Ultimate Pong Supreme Battle Royale Deluxe 2");
    screenSize = sf::Vector2f(window.getSize());

#pragma region Initialize Winsock, create socket, and setup server adress

    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // Winsock initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }

    // Socket creation
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    // Server adress configuration
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Erreur de conversion d'adresse IP !" << std::endl;
        return 1;
    }

#pragma endregion

#pragma region Connect and send message to introduce itself to the server

    std::cout << "Tentative de connexion au serveur..." << std::endl;

    std::string message = std::to_string(0) + " " + std::to_string(0);
    if (!sendto(clientSocket, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)))
    {
        std::cerr << "Impossible d'envoyer un message au serveur !" << std::endl;
        return 1;
    }

    std::cout << "Demande de connexion envoyee au serveur." << std::endl;

#pragma endregion

#pragma region Receive message from server and get ID and position from the server

    int playerID = 0;
    float paddleX = 0;
    float paddleY = 0;

    int serverAddrSize = sizeof(serverAddr);
    int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
    if (bytesReceived > 0)
    {
        buffer[bytesReceived] = '\0';

        // Getting ID and position
        sscanf_s(buffer, "%d %f %f", &playerID, &paddleX, &paddleY);
        std::cout << "Vous etes le joueur " << playerID << ", votre paddle se situe en position " << paddleX << "," << paddleY <<std::endl;
    }

#pragma endregion

    Paddle playerPaddle = Paddle(paddleX, paddleY, 20, 80, 500, screenSize);
#pragma region Manage gameplay and graphics with SFML and send and receive data from server to keep being updated
/*
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Send its ID and position to the server to keep it updated
        std::string message = std::to_string(playerID) + " " + 
            std::to_string(playerPaddle.GetPosition().x) + " " + std::to_string(playerPaddle.GetPosition().y);

        sendto(clientSocket, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));

        // À FAIRE : Gérer la réception de la position de l'adversaire et de la balle, et des autres données importantes
        //
        //

        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        //{
        //    playerPaddle.Move(-1);
        //}
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        //{
        //    playerPaddle.Move(1);
        //}

        window.clear();
        playerPaddle.Draw(&window);
        window.display();
    }
*/
#pragma endregion
    
#pragma region NewSFMLWindow
    App app = App();
    app.Run();
#pragma endregion
    
#pragma region Winsock cleanup

    closesocket(clientSocket);
    WSACleanup();

#pragma endregion

    return 0;
}
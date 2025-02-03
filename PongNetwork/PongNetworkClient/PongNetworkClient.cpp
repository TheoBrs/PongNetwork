#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SFML/Graphics.hpp>

#pragma comment(lib, "ws2_32.lib") // Lien avec la bibliothèque Winsock

#define SERVER_IP "127.0.0.1"
#define PORT 54000
#define BUFFER_SIZE 1024

int ConnectToServer()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // 1. Initialiser Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }

    // 2. Créer un socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. Configurer l'adresse du serveur
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Erreur de conversion d'adresse IP !" << std::endl;
        return 1;
    }

    // 4. Envoyer un message au serveur
    std::cout << "Tentative de connexion au serveur..." << std::endl;

    std::string message = "Hello, server !";
    if (!sendto(clientSocket, message.c_str(), message.size(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr)))
    {
        std::cerr << "Impossible d'envoyer un message au serveur !" << std::endl;
        return 1;
    }

    std::cout << "Message envoye au serveur." << std::endl;

    // 5. Recevoir un message du serveur
    int serverAddrSize = sizeof(serverAddr);
    int bytesReceived = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &serverAddrSize);
    if (bytesReceived > 0) 
    {
        buffer[bytesReceived] = '\0';
        std::cout << "Reponse du serveur : " << buffer << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

int main()
{
    ConnectToServer();

    sf::RenderWindow window(sf::VideoMode({ 200, 200 }), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
}
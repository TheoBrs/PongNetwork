#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <SFML/Graphics.hpp>

#pragma comment(lib, "ws2_32.lib") // Lien avec la bibliothèque Winsock

#define SERVER_IP "127.0.0.1"
#define PORT 54000
#define _WINSOCK_DEPRECATED_NO_WARNINGS

int ConnectToServer()
{
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;

    // 1. Initialiser Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }

    // 2. Créer un socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. Configurer l'adresse du serveur
    serverAddr.sin_family = AF_INET;

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Erreur de conversion d'adresse IP !" << std::endl;
        return 1;
    }

    serverAddr.sin_port = htons(PORT);

    // 4. Se connecter au serveur

    std::cout << "Tentative de connexion au serveur..." << std::endl;

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur connexion !" << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connecte au serveur !" << std::endl;

    // 5. Recevoir un message du serveur
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Message recu du serveur : " << buffer << std::endl;
    }

    // 6. Envoyer un message au serveur
    const char* msg = "Hello Serveur !";
    send(clientSocket, msg, strlen(msg), 0);

    // 7. Fermer le socket
    closesocket(clientSocket);
    WSACleanup();
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
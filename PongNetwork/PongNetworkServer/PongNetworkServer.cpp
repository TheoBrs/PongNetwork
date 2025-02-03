#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Lien avec la bibliothèque Winsock

#define PORT 54000

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);

    // 1. Initialiser Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }

    // 2. Créer un socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    // 3. Configurer l'adresse et le port
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 4. Associer le socket à l'adresse et au port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Erreur bind !" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 5. Passer en mode écoute
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Erreur listen !" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Serveur en attente sur le port " << PORT << "..." << std::endl;

    // 6. Accepter une connexion client
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Erreur accept !" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Client connecte !" << std::endl;

    // 7. Envoyer un message au client
    const char* welcomeMsg = "Bienvenue sur le serveur !";
    send(clientSocket, welcomeMsg, strlen(welcomeMsg), 0);

    // 8. Recevoir un message du client
    char buffer[512];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Message recu du client : " << buffer << std::endl;
    }

    // 9. Fermer les sockets
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    while (true)
    {

    }
}

#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib") // Lien avec la bibliothèque Winsock

#define PORT 54000
#define BUFFER_SIZE 1024

int main()
{
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    int clientAddrSize = sizeof(clientAddr);

    // 1. Initialiser Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }

    // 2. Créer un socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
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
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
    {
        std::cerr << "Erreur bind !" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // 5. Passer en mode écoute
    std::cout << "Serveur UDP en attente de messages..." << std::endl;
    while (true) 
    {
        // 6. Recevoir un message
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived > 0) 
        {
            buffer[bytesReceived] = '\0';
            std::cout << "Recu: " << buffer << std::endl;

            // 7. Envoyer une réponse
            std::string response = "Message recu !";
            sendto(serverSocket, response.c_str(), response.size(), 0, (sockaddr*)&clientAddr, clientAddrSize);
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    while (true)
    {

    }
}

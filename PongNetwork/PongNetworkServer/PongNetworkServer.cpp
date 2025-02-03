#include <iostream>
#include <winsock2.h>
#include <unordered_map>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define PORT 54000
#define BUFFER_SIZE 1024

struct Player 
{
    sockaddr_in addr;
    float paddleY;
};

std::unordered_map<int, Player> players;

int main()
{
#pragma region Initialize Winsock, create socket, and setup server adress and port
    WSADATA wsaData;
    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    int clientAddrSize = sizeof(clientAddr);

    // Winsock initialization
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        std::cerr << "Erreur Winsock !" << std::endl;
        return 1;
    }

    // Socket creation
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Erreur socket !" << std::endl;
        WSACleanup();
        return 1;
    }

    // Server adress and port configuration
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Associate socket to adress and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
    {
        std::cerr << "Erreur bind !" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
#pragma endregion

#pragma region While loop to listen to any messages and manage data

    std::cout << "Serveur en attente de joueurs..." << std::endl;

    float ballX = 400, ballY = 300;

    while (true) 
    {
        // Search for messages to receive
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived > 0) 
        {
            buffer[bytesReceived] = '\0';

            int playerID;
            float paddleY;
            sscanf_s(buffer, "%d %f", &playerID, &paddleY);

            // Add player if he's not already connected, or update its position
            if (playerID == 0) 
            {
                playerID = players.size() + 1;
                players[playerID] = { clientAddr, paddleY };
                std::cout << "Joueur " << playerID << " connecte !" << std::endl;

                if (playerID == 1)
                {
                    paddleY = 200;
                }
                else if (playerID == 2)
                {
                    paddleY = 600;
                }

                // Send the player its ID and position
                std::string message = std::to_string(playerID) + " " + std::to_string(paddleY);
                sendto(serverSocket, message.c_str(), message.size(), 0, (sockaddr*)&players[playerID].addr, sizeof(players[playerID].addr));
            }
            else 
            {
                players[playerID].paddleY = paddleY;
            }

            // Send updates to every players
            for (auto& [id, player] : players) 
            {
                std::string message = std::to_string(playerID) + " " + std::to_string(paddleY) + " " +
                    std::to_string(ballX) + " " + std::to_string(ballY);

                sendto(serverSocket, message.c_str(), message.size(), 0, (sockaddr*)&player.addr, sizeof(player.addr));
            }
        }
    }

#pragma endregion

#pragma region Winsock cleanup

    closesocket(serverSocket);
    WSACleanup();

#pragma endregion

    return 0;
}

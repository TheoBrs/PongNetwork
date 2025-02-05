#include <iostream>
#include <winsock2.h>
#include <unordered_map>
#include <string>
#include <SFML/System/Vector2.hpp>

#pragma comment(lib, "ws2_32.lib")

#define PORT 54000
#define BUFFER_SIZE 1024

struct Player 
{
    sockaddr_in addr;
    sf::Vector2f playerPosition;
    float inputMove =0.0f;
};

std::unordered_map<int, Player> players;


void SendPadlePositions(SOCKET serverSocket)
{
    for (auto playerToSend: players)
    {
        for (auto playerInfo: players)
        {
            sf::Vector2f pos = playerInfo.second.playerPosition;
            std::string messagePadle = "Padle " + std::to_string(playerInfo.first) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(playerInfo.second.inputMove);
            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second.addr, sizeof(playerToSend.second.addr));
        }
    }
}

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

    float ballX = 400, ballY = 400;
    float ballDirectionX = 3.2, ballDirectionY = 2.8;
    int playerID = 0;
    float paddleX = -1;
    float paddleY = -1;
    
    while (true) 
    {
        // Search for messages to receive
        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
        if (bytesReceived <= 0)
        {
            wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
            continue;
        }
        buffer[bytesReceived] = '\0';
        std::string messageReceive(buffer);
        auto packetType = messageReceive.substr(0, messageReceive.find(' '));
        
#pragma region Connection Request
        if (packetType == "ConnectionRequest")
        {
            std::cout << "Demande de connexion" << std::endl;
            if (players.size() < 2)
            {
                if (playerID == 0)
                {
                    paddleX = 100;
                    paddleY = 350;
                }else if (playerID == 1)
                {
                    paddleX = 1820;
                    paddleY = 350;
                }
                players[playerID] = {clientAddr, sf::Vector2f(paddleX, paddleY), 0};
                std::string messageToSend("ConnectionResponse 0");
                messageToSend += " " + std::to_string(playerID);
                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&players[playerID].addr, sizeof(players[playerID].addr));
                
                SendPadlePositions(serverSocket);
                playerID++;
            }
            else
            {
                std::string messageToSend("ConnectionResponse 1");
                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&clientAddr, sizeof(clientAddr));
            }

        }
#pragma endregion

#pragma region InputMove
        if (packetType == "InputMove")
        {
            char type[50];
            int clientId;
            float upAxis;
            float posX, posY;
            sscanf_s(buffer, "%s %d %f %f %f", &type, (unsigned)_countof(type), &clientId, &upAxis, &posX, &posY);
            players[clientId].playerPosition = sf::Vector2f(posX, posY);
            players[clientId].inputMove = upAxis;
            //players[clientId].playerPosition.y += upAxis;
            SendPadlePositions(serverSocket);
        }
#pragma endregion 
        
#pragma region Old Code
        // int playerID = -1;
        // float paddleX = -1;
        // float paddleY = -1;
        //
        // sscanf_s(buffer, "%d %f", &playerID, &paddleX);
        //
        // // Add player if he's not already connected and there's less than 2 players, 
        // // or else update its position
        // if (playerID == 0 && players.size() <= 2)
        // {
        //     playerID = players.size() + 1;
        //     
        //     std::cout << "Joueur " << playerID << " connecte !" << std::endl;
        //
        //     if (playerID == 1)
        //     {
        //         paddleX = 100;
        //         paddleY = 350;
        //     }
        //     else if (playerID == 2)
        //     {
        //         paddleX = 700;
        //         paddleY = 350;
        //     }
        //
        //     // Add player to the unordered_map
        //     players[playerID] = { clientAddr, sf::Vector2f(paddleX, paddleY)};
        //
        //     // Send the player its ID and position, and the position and speed of the ball
        //     std::string message = std::to_string(playerID) + " " + std::to_string(paddleX) + " " + std::to_string(paddleY) +
        //         " " + std::to_string(ballX) + " " + std::to_string(ballY) + " " + std::to_string(ballDirectionX) + " " + std::to_string(ballDirectionY);
        //
        //     sendto(serverSocket, message.c_str(), message.size(), 0, (sockaddr*)&players[playerID].addr, sizeof(players[playerID].addr));
        // }
        // else 
        // {
        //     players[playerID].playerPosition.x = paddleX;
        //     players[playerID].playerPosition.y = paddleY;
        // }
        //
        //
        //
        // // Send updates to every players
        // for (auto& [id, player] : players) 
        // {
        //     std::string message = std::to_string(playerID) + " " + std::to_string(paddleX) + " " + std::to_string(paddleY) +
        //         " " + std::to_string(ballX) + " " + std::to_string(ballY) + " " + std::to_string(ballDirectionX) + " " + std::to_string(ballDirectionY);
        //
        //     sendto(serverSocket, message.c_str(), message.size(), 0, (sockaddr*)&player.addr, sizeof(player.addr));
        // }
#pragma endregion 
        
    }

#pragma endregion

#pragma region Winsock cleanup

    closesocket(serverSocket);
    WSACleanup();

#pragma endregion

    return 0;
}

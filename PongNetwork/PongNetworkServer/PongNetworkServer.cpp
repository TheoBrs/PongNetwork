//
//#pragma region Server Without Simulation
//
//#include <iostream>
//#include <winsock2.h>
//#include <unordered_map>
//#include <string>
//#include <SFML/System/Vector2.hpp>
//#include <chrono>
//
//#pragma comment(lib, "ws2_32.lib")
//
//#define PORT 54000
//#define BUFFER_SIZE 1024
//
//struct Player 
//{
//    sockaddr_in addr;
//    sf::Vector2f playerPosition;
//    float inputMove = 0.0f;
//    int score = 0;
//    std::string name;
//};
//
//auto m_scoreClock = std::chrono::system_clock::now();
//std::unordered_map<int, Player*> players;
//
//int playerID = 0;
//int m_maxNumberOfPlayer = 2;
//int scoreClock = 0;
//
//float ballX = 800, ballY = 450;
//float ballDirectionX = 4.3, ballDirectionY = 3.7;
//float paddleX = -1;
//float paddleY = -1;
//
//bool m_fullLobbyMessageSent = false;
//
//void SendPaddlesPositions(SOCKET serverSocket)
//{
//    for (auto playerToSend: players)
//    {
//        for (auto playerInfo: players)
//        {
//            sf::Vector2f pos = playerInfo.second->playerPosition;
//            std::string messagePadle = "Paddle " + std::to_string(playerInfo.first) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(playerInfo.second->inputMove);
//            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
//        }
//    }
//}
//
//void SendPaddleWithId(SOCKET serverSocket, int clientid)
//{
//    for (auto playerToSend: players)
//    {
//        sf::Vector2f pos = players[clientid]->playerPosition;
//        std::string messagePadle = "Paddle " + std::to_string(clientid) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(players[clientid]->inputMove);
//        sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
//
//    }
//}
//
//void SendNewPlayer(SOCKET serverSocket)
//{
//    for (auto playerToSend: players)
//    {
//        for (auto playerInfo: players)
//        {
//            sf::Vector2f pos = playerInfo.second->playerPosition;
//            std::string messagePadle =
//                "NewPlayer " + std::to_string(playerInfo.first) + " " +
//                std::to_string(pos.x) + " " +
//                std::to_string(pos.y) + " " +
//                std::to_string(playerInfo.second->inputMove) + " " +
//                playerInfo.second->name;
//            
//            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
//        }
//    }
//}
//
//void SendBallPosition(SOCKET serverSocket)
//{
//    for (auto playerToSend : players)
//    {
//        std::string messageToSend("Ball");
//        messageToSend += " " + std::to_string(ballX) + " " + std::to_string(ballY) + " " + std::to_string(ballDirectionX) + " " + std::to_string(ballDirectionY);
//        sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
//    }
//}
//
//void SendScore(SOCKET serverSocket)
//{
//    for (auto playerToSend : players)
//    {
//        for (auto playerInfo : players)
//        {
//            int playerScore = playerInfo.second->score;
//
//            std::string messagePadle = "Score " + std::to_string(playerInfo.first) + " " + std::to_string(playerScore);
//            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
//        }
//    }
//}
//
//void Respawn(SOCKET serverSocket)
//{
//    // Reset player positions
//    for (auto player: players)
//    {
//        if (player.first == 0)
//        {
//            std::cout << "Player " << player.first << " position reset : 100, 440" << std::endl;
//            player.second->playerPosition.x = 100;
//            player.second->playerPosition.y = 400;
//        }
//        else if (player.first == 1)
//        {
//            std::cout << "Player " << player.first << " position reset : 1820, 440" << std::endl;
//            player.second->playerPosition.x = 1500;
//            player.second->playerPosition.y = 400;
//        }
//    }
//
//    // Reset ball position
//    ballX = 800, ballY = 450;
//
//    // Send every new informations to every players
//    SendBallPosition(serverSocket);
//    SendPaddlesPositions(serverSocket);
//    SendScore(serverSocket);
//}
//
//int main()
//{
//#pragma region Initialize Winsock, create socket, and setup server adress and port
//    WSADATA wsaData;
//    SOCKET serverSocket;
//    sockaddr_in serverAddr, clientAddr;
//    char buffer[BUFFER_SIZE];
//    int clientAddrSize = sizeof(clientAddr);
//
//    // Winsock initialization
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
//    {
//        std::cerr << "Erreur Winsock !" << std::endl;
//        return 1;
//    }
//
//    // Socket creation
//    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
//    if (serverSocket == INVALID_SOCKET) {
//        std::cerr << "Erreur socket !" << std::endl;
//        WSACleanup();
//        return 1;
//    }
//
//    // Server adress and port configuration
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_addr.s_addr = INADDR_ANY;
//    serverAddr.sin_port = htons(PORT);
//
//    // Associate socket to adress and port
//    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
//    {
//        std::cerr << "Erreur bind !" << std::endl;
//        closesocket(serverSocket);
//        WSACleanup();
//        return 1;
//    }
//#pragma endregion
//
//#pragma region While loop to listen to any messages and manage data
//
//    std::cout << "Serveur en attente de joueurs..." << std::endl;
//    
//    while (true) 
//    {
//        // Search for messages to receive
//        int bytesReceived = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&clientAddr, &clientAddrSize);
//        if (bytesReceived <= 0)
//        {
//            wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
//            continue;
//        }
//        buffer[bytesReceived] = '\0';
//        std::string messageReceive(buffer);
//        auto packetType = messageReceive.substr(0, messageReceive.find(' '));
//        
//#pragma region Connection Request
//        if (packetType == "ConnectionRequest")
//        {
//            std::cout << "Demande de connexion" << std::endl;
//            if (players.size() < m_maxNumberOfPlayer)
//            {
//                char type[50];
//                char name[100];
//                sscanf_s(buffer, "%s %s", &type, (unsigned)_countof(type), &name,  (unsigned)_countof(name));
//
//                
//                if (playerID == 0)
//                {
//                    paddleX = 100;
//                    paddleY = 400;
//                }
//                else if (playerID == 1)
//                {
//                    paddleX = 1500;
//                    paddleY = 400;
//                }
//                players[playerID] =
//                    new Player{clientAddr, sf::Vector2f(paddleX, paddleY), 0,0,name};
//                std::string messageToSend("ConnectionResponse 0");
//                messageToSend += " " + std::to_string(playerID);
//                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&players[playerID]->addr, sizeof(players[playerID]->addr));
//                SendNewPlayer(serverSocket);
//                //SendPaddlesPositions(serverSocket);
//                playerID++;
//            }
//            else
//            {
//                std::string messageToSend("ConnectionResponse 1");
//                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&clientAddr, sizeof(clientAddr));
//            }
//        }
//#pragma endregion
//
//#pragma region Player scored
//
//        if (packetType == "Score" && std::chrono::system_clock::now() - m_scoreClock > std::chrono::seconds(1))
//        {
//            m_scoreClock = std::chrono::system_clock::now();
//
//            char type[50];
//            int playerWhoScored;
//            sscanf_s(buffer, "%s %d", &type, (unsigned)_countof(type), &playerWhoScored);
//
//            // Add a point to the score of the player who scored
//            players[playerWhoScored]->score += 1;
//
//            std::cout << "Le joueur " << playerWhoScored << " a marque !" << std::endl;
//            
//            // Respawn paddles, ball, and send new score to every players
//            Respawn(serverSocket);
//        }
//
//#pragma endregion
//
//#pragma region LobbyFull message and ball initialization
//
//        if (players.size() == m_maxNumberOfPlayer && !m_fullLobbyMessageSent)
//        {
//            m_fullLobbyMessageSent = true;
//
//            for (auto playerToSend : players)
//            {
//                std::string messageToSend("LobbyFull");
//                messageToSend += " " + std::to_string(ballX) + " " + std::to_string(ballY) +
//                    " " + std::to_string(ballDirectionX) + " " + std::to_string(ballDirectionY);
//                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
//            }
//        }
//
//#pragma endregion
//
//#pragma region InputMove
//        if (packetType == "InputMove")
//        {
//            char type[50];
//            int clientId;
//            float upAxis;
//            float posX, posY;
//            sscanf_s(buffer, "%s %d %f %f %f", &type, (unsigned)_countof(type), &clientId, &upAxis, &posX, &posY);
//            players[clientId]->playerPosition = sf::Vector2f(posX, posY);
//            players[clientId]->inputMove = upAxis;
//            //players[clientId].playerPosition.y += upAxis;
//            SendPaddleWithId(serverSocket, clientId);
//        }
//#pragma endregion 
//        
//    }
//
//#pragma endregion
//
//#pragma region Winsock cleanup
//
//    closesocket(serverSocket);
//    WSACleanup();
//
//#pragma endregion
//
//    return 0;
//}
//
//#pragma endregion
//


#pragma region Server With Simulation
#include "Server.h"

struct Player 
{
    sockaddr_in addr;
    sf::Vector2f playerPosition;
    float inputMove = 0.0f;
    int score = 0;
    std::string name;
    bool isConnected = false;
};

std::unordered_map<int, Player*> players;
std::unordered_map<int, std::chrono::time_point<std::chrono::system_clock>> m_playerLastResponse;
std::chrono::time_point<std::chrono::system_clock> m_scoreClock = std::chrono::system_clock::now();

int playerID = 0;
int m_maxNumberOfPlayer = 2;
int m_numberOfConnectedPlayers = 0;
int scoreClock = 0;

float ballX = 800, ballY = 450;
float ballDirectionX = 4.3, ballDirectionY = 3.7;
float m_firstPlayerX = 100, m_firstPlayerY = 400;
float m_secondPlayerX = 1500, m_secondPlayerY = 400;

bool m_fullLobbyMessageSent = false;

void SendPaddlesPositions(SOCKET serverSocket)
{
    for (auto playerToSend: players)
    {
        for (auto playerInfo: players)
        {
            sf::Vector2f pos = playerInfo.second->playerPosition;
            std::string messagePadle = "Paddle " + std::to_string(playerInfo.first) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(playerInfo.second->inputMove);
            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
        }
    }
}

void SendPaddleWithId(SOCKET serverSocket, int clientid)
{
    for (auto playerToSend: players)
    {
        sf::Vector2f pos = players[clientid]->playerPosition;
        std::string messagePadle = "Paddle " + std::to_string(clientid) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(players[clientid]->inputMove);
        sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));

    }
}

void SendNewPlayer(SOCKET serverSocket)
{
    for (auto playerToSend: players)
    {
        for (auto playerInfo: players)
        {
            sf::Vector2f pos = playerInfo.second->playerPosition;
            std::string messagePadle =
                "NewPlayer " + std::to_string(playerInfo.first) + " " +
                std::to_string(pos.x) + " " +
                std::to_string(pos.y) + " " +
                std::to_string(playerInfo.second->inputMove) + " " +
                playerInfo.second->name;
            
            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
        }
    }
}

void SendBallPosition(SOCKET serverSocket)
{
    for (auto playerToSend : players)
    {
        std::string messageToSend("Ball");
        messageToSend += " " + std::to_string(ballX) + " " + std::to_string(ballY) + " " + std::to_string(ballDirectionX) + " " + std::to_string(ballDirectionY);
        sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
    }
}

void SendScore(SOCKET serverSocket)
{
    for (auto playerToSend : players)
    {
        for (auto playerInfo : players)
        {
            int playerScore = playerInfo.second->score;

            std::string messagePadle = "Score " + std::to_string(playerInfo.first) + " " + std::to_string(playerScore);
            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
        }
    }
}

void Respawn(SOCKET serverSocket)
{
    // Reset player positions
    for (auto player: players)
    {
        if (player.first == 0)
        {
            std::cout << "Player " << player.first << " position reset : 100, 440" << std::endl;
            player.second->playerPosition.x = m_firstPlayerX;
            player.second->playerPosition.y = m_firstPlayerY;
        }
        else if (player.first == 1)
        {
            std::cout << "Player " << player.first << " position reset : 1820, 440" << std::endl;
            player.second->playerPosition.x = m_secondPlayerX;
            player.second->playerPosition.y = m_secondPlayerY;
        }
    }

    // Send every new informations to every players
    SendBallPosition(serverSocket);
    SendPaddlesPositions(serverSocket);
    SendScore(serverSocket);
}

void PlayerDisconnected(SOCKET serverSocket, int disconnectedPlayerID)
{
    m_fullLobbyMessageSent = false;

    // Send a message to every player still connected to stop the game and erase the disconnected player
    for (auto player : players)
    {
        player.second->score = 0;

        if (player.second->isConnected)
        {
            if (player.first == 0)
            {
                player.second->playerPosition.x = m_firstPlayerX;
                player.second->playerPosition.y = m_firstPlayerY;
            }
            else if (player.first == 1)
            {
                player.second->playerPosition.x = m_secondPlayerX;
                player.second->playerPosition.y = m_secondPlayerY;
            }

            SendPaddlesPositions(serverSocket);
            SendScore(serverSocket);

            std::string messagePadle = "PlayerDisconnected " + std::to_string(disconnectedPlayerID);
            sendto(serverSocket, messagePadle.c_str(), messagePadle.size(), 0, (sockaddr*)&player.second->addr, sizeof(player.second->addr));
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

            // If the lobby isn't full, add the player
            if (players.size() < m_maxNumberOfPlayer)
            {
                char type[50];
                char name[100];
                sscanf_s(buffer, "%s %s", &type, (unsigned)_countof(type), &name,  (unsigned)_countof(name));

                float paddleX, paddleY;

                if (playerID == 0)
                {
                    paddleX = m_firstPlayerX;
                    paddleY = m_firstPlayerY;
                }
                else if (playerID == 1)
                {
                    paddleX = m_secondPlayerX;
                    paddleY = m_secondPlayerY;
                }

                players[playerID] =
                    new Player{ clientAddr, sf::Vector2f(paddleX, paddleY), 0, 0, name, true };

                m_playerLastResponse[playerID] = std::chrono::system_clock::now();

                std::string messageToSend("ConnectionResponse 0");
                messageToSend += " " + std::to_string(playerID);
                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&players[playerID]->addr, sizeof(players[playerID]->addr));
                SendNewPlayer(serverSocket);
                playerID++;
                m_numberOfConnectedPlayers++;
            }
            // If a player disconnected, we give the new client the ID of the player who disconnected
            else if (m_numberOfConnectedPlayers != m_maxNumberOfPlayer)
            {
                char type[50];
                char name[100];
                sscanf_s(buffer, "%s %s", &type, (unsigned)_countof(type), &name, (unsigned)_countof(name));

                float paddleX, paddleY;

                for (auto player : players)
                {
                    if (!player.second->isConnected)
                    {
                        if (player.first == 0)
                        {
                            paddleX = m_firstPlayerX;
                            paddleY = m_firstPlayerY;
                        }
                        else if (player.first == 1)
                        {
                            paddleX = m_secondPlayerX;
                            paddleY = m_secondPlayerY;
                        }

                        players[player.first] =
                            new Player{ clientAddr, sf::Vector2f(paddleX, paddleY), 0, 0, name, true };

                        m_playerLastResponse[player.first] = std::chrono::system_clock::now();

                        std::string messageToSend("ConnectionResponse 0");
                        messageToSend += " " + std::to_string(player.first);
                        sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&players[player.first]->addr, sizeof(players[player.first]->addr));
                        SendNewPlayer(serverSocket);
                    }
                }

                m_numberOfConnectedPlayers++;
            }
            else
            {
                std::string messageToSend("ConnectionResponse 1");
                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&clientAddr, sizeof(clientAddr));
            }
        }
#pragma endregion

#pragma region Player scored

        if (packetType == "Score" && std::chrono::system_clock::now() - m_scoreClock > std::chrono::seconds(1) && m_fullLobbyMessageSent)
        {
            m_scoreClock = std::chrono::system_clock::now();

            char type[50];
            int playerWhoScored;
            sscanf_s(buffer, "%s %d", &type, (unsigned)_countof(type), &playerWhoScored);

            // Add a point to the score of the player who scored
            players[playerWhoScored]->score += 1;

            std::cout << "Le joueur " << playerWhoScored << " a marque !" << std::endl;
            
            // Respawn paddles, ball, and send new score to every players
            Respawn(serverSocket);
        }

#pragma endregion

#pragma region LobbyFull message and ball initialization


        if (players.size() == m_maxNumberOfPlayer && !m_fullLobbyMessageSent && m_numberOfConnectedPlayers == m_maxNumberOfPlayer)
        {
            m_fullLobbyMessageSent = true;

            for (auto playerToSend : players)
            {
                std::string messageToSend("LobbyFull");
                messageToSend += " " + std::to_string(ballX) + " " + std::to_string(ballY) +
                    " " + std::to_string(ballDirectionX) + " " + std::to_string(ballDirectionY);
                sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&playerToSend.second->addr, sizeof(playerToSend.second->addr));
            }
        }

#pragma endregion

#pragma region Player connection ping

        if (packetType == "Ping")
        {
            char type[50];
            int playerID;
            sscanf_s(buffer, "%s %d", &type, (unsigned)_countof(type), &playerID);

            // Reset inactivity clock for this player
            m_playerLastResponse[playerID] = std::chrono::system_clock::now();
        }

        for (auto player : players)
        {
            if (player.second->isConnected)
            {
                // If a client didn't ping the server for 7 seconds, disconnect it
                if (std::chrono::system_clock::now() - m_playerLastResponse[player.first] > std::chrono::seconds(7))
                {
                    std::cout << "Joueur " << player.first << " est deconnecte !" << std::endl;
                    player.second->isConnected = false;
                    m_numberOfConnectedPlayers--;
                    PlayerDisconnected(serverSocket, player.first);
                }
                // If a client didn't ping the server for 5 seconds, send a last chance ping to the client
                else if (std::chrono::system_clock::now() - m_playerLastResponse[player.first] > std::chrono::seconds(5))
                {
                    std::cout << "LactChancePing sent." << std::endl;

                    std::string messageToSend("LastChancePing");
                    sendto(serverSocket, messageToSend.c_str(), messageToSend.size(), 0, (sockaddr*)&player.second->addr, sizeof(player.second->addr));
                }
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
            players[clientId]->playerPosition = sf::Vector2f(posX, posY);
            players[clientId]->inputMove = upAxis;
            //players[clientId].playerPosition.y += upAxis;
            SendPaddleWithId(serverSocket, clientId);
        }
#pragma endregion 
    }

#pragma endregion

#pragma region Winsock cleanup

    closesocket(serverSocket);
    WSACleanup();

#pragma endregion

    return 0;
}
#pragma endregion
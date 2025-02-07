#include <SFML/Graphics.hpp>
#include <iostream>
#include "Client.h"
#include "UDPClient.h"
#include "Menu.h"
#include "EventHandler.h"

#include "Game.h"
#include "../PongGame/Ball.h"
#include "../PongGame/Paddle.h"
#include "PLayerController.h"
#include "UserInterface.h"

sf::Font* Client::s_MainFont = nullptr;

PacketType Client::ResolvePacketType(const std::string& input)
{
	if (input == "ConnectionResponse") return ConnectionResponse;
	if (input == "GameSettings") return GameSettings;
	if (input == "NewPlayer") return NewPlayer;
	if (input == "Ball") return Ball;
	if (input == "Paddle") return Paddle;
	if (input == "Score") return Score;
	if (input == "IsGameRunning") return IsGameRunning;
	if (input == "Pong") return PongType;
	if (input == "Disconnection") return Disconnection;
	return PacketType::Invalid;
}

void Client::Init()
{
	m_udpClient = new UDPClient();
	m_udpClient->Init();

	s_MainFont = new sf::Font();
	if (!s_MainFont->openFromFile("Fonts/arial/arial.ttf"))
	{
		std::cout << "Failed to load font\n";
		return;
	}

	m_window = new sf::RenderWindow(
		sf::VideoMode({ 1600, 900 }), "Pong", sf::Style::Titlebar | sf::Style::Close);
	m_window->setFramerateLimit(60);
	EventHandler::Init(m_window);

	m_menu = new Menu();
	m_menu->Init(s_MainFont);
	m_menu->OnValidateEvent += [this](std::string ip, int port, std::string name)
		{
			Connection(ip, port, name);
		};
	m_menu->SetActive(true);
	m_game = new Pong::Game();
	m_game->SetIsGameRunning(false);

	m_playerController = new PLayerController();
	m_playerController->Init();
	m_playerController->OnChangeInput += [this](float input) {SendOnChangeInput(input); };

	m_userInterface = new UserInterface();
	m_userInterface->Init(s_MainFont);

	m_pingTime = new sf::Clock();
	m_pingTime->reset();
}

void Client::Update(float deltaTime)
{
	if (!m_isConnected)
	{
		return;
	}

	if (m_pingTime->getElapsedTime().asSeconds() >= 2.f)
	{
		m_pingTime->restart();
		SendPing();
	}

	if (!m_isInGame)
	{
		return;
	}

	m_game->Update(deltaTime);
}

void Client::Draw()
{
	m_window->clear();
	if (!m_isInGame)
	{
		m_window->draw(*m_menu);
	}
	else
	{
		m_window->draw(*m_game);
		m_window->draw(*m_userInterface);
	}
	m_window->display();
}

void Client::HandleSfmlEvents()
{
	EventHandler::HandleEvent();
}

void Client::Connection(std::string ip, int port, std::string name)
{
	std::cout 
		<< "Try connection: \nIP: " << ip << ", Port: " << std::to_string(port) << ", Name: " << name + "\n";
	m_udpClient->TryConnect(ip, port, name);
}

void Client::SendOnChangeInput(float input)
{
	std::string messageToSend = "InputChange " + std::to_string(m_clientId) + " " + std::to_string(input);
	m_udpClient->AddMessageToSend(messageToSend);
}

void Client::SendPing()
{
	m_pingTime->restart();
	std::string messageToSend = "Ping " + std::to_string(m_clientId);
	m_udpClient->AddMessageToSend(messageToSend);
}

void Client::GetMessages()
{
	int byteReceived ;
	do
	{
		char buffer[BUFFER_SIZE];
		byteReceived = m_udpClient->ReceiveMessage(buffer);
		if (byteReceived != SOCKET_ERROR)
		{
			HandleMessage(buffer);
		}
	} while (byteReceived != SOCKET_ERROR);
}

void Client::HandleMessage(char(&buffer)[BUFFER_SIZE])
{
	std::string message(std::begin(buffer), std::end(buffer));
	auto packetType = message.substr(0, message.find(' '));
	PacketType type = ResolvePacketType(packetType);

	switch (type)
	{
	case ConnectionResponse:
		HandleMessageConnectionResponse(buffer);
		break;
	case GameSettings:
		HandleMessageGameSettings(buffer);
		break;
	case NewPlayer:
		HandleMessageNewPlayer(buffer);
		break;
	case Ball:
		HandleMessageBall(buffer);
		break;
	case Paddle:
		HandleMessagePaddle(buffer);
		break;
	case Score:
		HandleMessageScore(buffer);
		break;
	case IsGameRunning:
		HandleMessageIsGameRunning(buffer);
		break;
	case PongType:
		HandleMessagePong(buffer);
		break;
	case Disconnection:
		HandleMessageDisconnection(buffer);
	case Invalid:
		break;
	default:
		break;
	}
}
#pragma region HandleMessageMethods

void Client::HandleMessageConnectionResponse(char(&buffer)[BUFFER_SIZE])
{
	char type[50];
	int responseValue;
	int clientId;
	sscanf_s(buffer, "%s %d %d", &type, (unsigned)_countof(type), &responseValue, &clientId);
	if (responseValue != 0)
	{
		m_menu->SetErrorText("La connexion a échoué");
	}

	m_clientId = clientId;
	m_isConnected = true;
	m_pingTime->start();
}

void Client::HandleMessageGameSettings(char(&buffer)[BUFFER_SIZE])
{
	char type[50];
	float screenSizeX, screenSizeY;
	float paddleLeftPosX, paddleLeftPosY, paddleLeftSpeed, paddleLeftWidth, paddleLeftHeight;
	float paddleRightPosX, paddleRightPosY, paddleRightSpeed, paddleRightWidth, paddleRightHeight;
	float ballPosX, ballPosY, ballSpeed, ballRadius;

	sscanf_s(buffer, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f ", 
		&type, (unsigned)_countof(type),
		&screenSizeX, &screenSizeY,
		&paddleLeftPosX, &paddleLeftPosY, &paddleLeftSpeed, &paddleLeftWidth, &paddleLeftHeight,
		&paddleRightPosX, &paddleRightPosY, &paddleRightSpeed, &paddleRightWidth, &paddleRightHeight,
		&ballPosX, &ballPosY, &ballSpeed, &ballRadius
		);
	sf::Vector2f screenSize{ screenSizeX, screenSizeY };
	Pong::PaddleSettings paddleLeft{ {paddleLeftPosX, paddleLeftPosY}, paddleLeftSpeed, paddleLeftWidth, paddleLeftHeight };
	Pong::PaddleSettings paddleRight{ {paddleRightPosX, paddleRightPosY}, paddleRightSpeed, paddleRightWidth, paddleRightHeight };
	Pong::BallSettings ball{ {ballPosX, ballPosY}, ballSpeed, ballRadius };

	Pong::GameSettings gameSettings{screenSize, ball, paddleLeft, paddleRight};
	m_game->Init(gameSettings);
	m_isInGame = true;
	m_playerController->SetActive(true);
	m_menu->SetActive(false);
}

void Client::HandleMessageNewPlayer(char(&buffer)[BUFFER_SIZE])
{
	// to getting name for UI
	char type[50];
	char name[100];
	int clientId;
	sscanf_s(buffer, "%s %s %d", &type, (unsigned)_countof(type), &name, (unsigned)_countof(name), &clientId);
	std::string nameString(name);
	m_userInterface->AddPlayer(clientId, nameString, true);


}

void Client::HandleMessageBall(char(&buffer)[BUFFER_SIZE])
{
	char type[50];
	float ballDirX, ballDirY;
	float ballPosX, ballPosY;
	sscanf_s(buffer, "%s %f %f %f %f", &type, (unsigned)_countof(type), &ballDirX, &ballDirY, &ballPosX, &ballPosY);
	Pong::Ball* ball = m_game->GetBall();
	ball->SetDirection({ballDirX, ballDirY});
	ball->SetPosition({ ballPosX, ballPosY });
}

void Client::HandleMessagePaddle(char(&buffer)[BUFFER_SIZE])
{
	char type[50];
	int isLeftInt;
	float upAxis, posX, posY;
	sscanf_s(buffer, "%s %d %f %f %f", &type, (unsigned)_countof(type), &isLeftInt, &upAxis, &posX, &posY);
	bool isLeft;
	if (isLeftInt == 0 )
	{
		isLeft = false;
	}
	else if (isLeftInt == 1) 
	{
		isLeft = true;
	}

	Pong::Paddle* paddle = m_game->GetPaddle(isLeft);
	paddle->SetDirection(upAxis);
	paddle->SetPosition({ posX, posY });

}

void Client::HandleMessageScore(char(&buffer)[BUFFER_SIZE])
{
	char type[50];
	int scoreRight, scoreLeft;

	sscanf_s(buffer, "%s %d %d", &type, (unsigned)_countof(type), &scoreLeft, &scoreRight);

	m_game->SetScore(scoreLeft, scoreRight);
	m_userInterface->SetScore(scoreLeft, scoreRight);
}

void Client::HandleMessageIsGameRunning(char(&buffer)[BUFFER_SIZE])
{
	char type[50];
	int isGameRunningInt;
	sscanf_s(buffer, "%s %d", &type, (unsigned)_countof(type), &isGameRunningInt);
	bool isGameRunning = false;
	if (isGameRunningInt == 0)
	{
		isGameRunning = false;
	}
	else if (isGameRunningInt == 1)
	{
		isGameRunning = true;
	}
	m_game->SetIsGameRunning(isGameRunning);

}

void Client::HandleMessagePong(char(&buffer)[BUFFER_SIZE])
{
	sf::Time pingms = m_pingTime->getElapsedTime();
	
}

void Client::HandleMessageDisconnection(char(&buffer)[BUFFER_SIZE])
{
	char type[50];
	int playerId;
	sscanf_s(buffer, "%s %d", &type, (unsigned)_countof(type), &playerId);

	m_userInterface->SetPlayerConnection(playerId, false);

}

#pragma endregion

void Client::Run()
{
	Init();
	sf::Clock clock;
	while (m_window->isOpen())
	{
		GetMessages();
		sf::Time timeElapsed = clock.restart();
		HandleSfmlEvents();
		Update(timeElapsed.asSeconds());
		Draw();

		m_udpClient->SendMessagesUDP();
	}
	m_udpClient->UnInit();
}

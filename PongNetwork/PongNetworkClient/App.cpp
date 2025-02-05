#include <iostream>
#include <sstream>
#include <string>

#include "Button.h"
#include "App.h"
#include "EventHandler.h"
#include "TextField.h"
#include "UDPClient.h"
#include "Ball.h"

using namespace std::placeholders;

sf::RenderWindow* App::Window;

void App::Run()
{
	Init();
	while (Window->isOpen())
	{
		HandleServerMessages();
		HandleEvents();
		Update();
		Draw();
	}
	m_udpClient->UnInit();
}

sf::Vector2u App::GetWindowSize()
{
	return Window->getSize();
}

void App::HandleServerMessages()
{
	char buffer[BUFFER_SIZE];
	int result = m_udpClient->ReceiveMessage(buffer);
	if (result == SOCKET_ERROR)
	{
		//wprintf(L"recvfrom failed with error %d\n", WSAGetLastError());
		return;
	}
	std::string meesage(buffer);
	auto messageType = meesage.substr(0, meesage.find(' '));

	if (messageType == "ConnectionResponse")
	{
		char type[50];
		int responseValue;
		int clientId;
		sscanf_s(buffer, "%s %d %d", &type, (unsigned)_countof(type), &responseValue, &clientId);
		if (responseValue == 0)
		{
			// If we connect to server
			m_clientId = clientId;
			m_isServerJoined = true;
			JoinGame();
		}
	}
	else if (messageType == "Padle")
	{
		HandlePadleMessage(buffer);
	}
	else if (messageType == "Ball")
	{
		char type[50];
		float ballX, ballY;
		float ballDirectionX, ballDirectionY;
		sscanf_s(buffer, "%s %f %f %f %f", &type, (unsigned)_countof(type), &ballX, &ballY, &ballDirectionX, &ballDirectionY);

		ball = new Ball(ballX, ballY, 10, sf::Vector2f(ballDirectionX, ballDirectionY), sf::Vector2f(GetWindowSize()));
	}
	else if (messageType == "Score")
	{
		char type[50];
		int clientId;
		int newScore;
		sscanf_s(buffer, "%s %d %d", &type, (unsigned)_countof(type), &clientId, &newScore);

		for (auto player : m_players)
		{
			if (player.ClientId == clientId)
			{
				player.score = newScore;
				std::cout << "Player " << player.ClientId << " score is " << player.score << std::endl;
			}
		}
	}
	else if (messageType == "LobbyFull")
	{
		m_twoPlayerJoined = true;

		char type[50];
		float ballX, ballY;
		float ballDirectionX, ballDirectionY;
		sscanf_s(buffer, "%s %f %f %f %f", &type, (unsigned)_countof(type), &ballX, &ballY, &ballDirectionX, &ballDirectionY);

		ball = new Ball(ballX, ballY, 10, sf::Vector2f(ballDirectionX, ballDirectionY), sf::Vector2f(GetWindowSize()));
	}
}

void App::Init()
{
	
	m_udpClient = new UDPClient();
	m_udpClient->Init();
	Window = new sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "SFML works!");
	Window->setFramerateLimit(60);
	m_textField = new TextField();
	m_textField->Init(sf::Vector2f(760,245), sf::Vector2f(400,100));
	m_eventValidateTextId = m_textField->OnValidateText += [this](std::string text)
	{EventValidateTextCallback(text);};
	
}

void App::Update()
{
	if (m_twoPlayerJoined)
	{
		ball->Move();
		for (auto player : m_players)
		{
			ball->OnPaddleCollision(player.Character);
		}
		
		if (ball->GetPosition().x - (ball->GetShape().getRadius() * 2) <= 0)
		{
			playerWhoScored = 1;

			// Send which side the ball is to the server to determine who scored
			std::string messageToSend = "Score";
			messageToSend += " " + std::to_string(playerWhoScored);
			m_udpClient->SendMessageUDP(messageToSend);
		}

		if (ball->GetPosition().x >= GetWindowSize().x)
		{
			playerWhoScored = 0;

			// Send which side the ball is to the server to determine who scored
			std::string messageToSend = "Score";
			messageToSend += " " + std::to_string(playerWhoScored);
			m_udpClient->SendMessageUDP(messageToSend);
		}
	}
}

void App::Draw()
{
	Window->clear();
	if (!m_isServerJoined)
	{
		Window->draw(*m_textField);
	}
	else
	{
		for (auto player: m_players)
		{
			player.Character->Draw(Window);
		}

		if (m_twoPlayerJoined)
		{
			ball->Draw(Window);
		}

		// Dessiner le score à l'écran
	}
	Window->display();
}

void App::HandleEvents()
{
	EventHandler::HandleEvent(Window);
}

void App::JoinGame()
{
	m_eventplayerInputId = EventHandler::OnKeyPressed += [this](const sf::Event::KeyPressed* event)
	{EventKeyPressedCallback(event);};
	m_textField->OnValidateText -= m_eventValidateTextId;

	
}

void App::HandlePadleMessage(char messageBuffer[])
{
	char type[50];
	int clientId;
	float posX, posY;
	sscanf_s(messageBuffer, "%s %d %f %f %d", &type, (unsigned)_countof(type), &clientId, &posX, &posY);
	
	bool isPLayerInstanciated = false;
	for (auto player: m_players)
	{
		if (player.ClientId == clientId)
		{
			isPLayerInstanciated = true;
			player.Character->SetPosition(sf::Vector2f(posX, posY));
		}
	}

	if (!isPLayerInstanciated)
	{
		Player newPlayer = Player();
		newPlayer.ClientId = clientId;
		newPlayer.Character =
			new Paddle(posX, posY, 20, 80, 500, sf::Vector2f(Window->getSize()));
		m_players.push_back(newPlayer);
	}

}

void App::EventKeyPressedCallback(const sf::Event::KeyPressed* event)
{
	int upAxis = 0;
	int speed = 10;
	if (event->scancode == sf::Keyboard::Scancode::Up)
	{
		upAxis-= 1 * speed;
	}
	if (event->scancode == sf::Keyboard::Scancode::Down)
	{
		upAxis+= 1 * speed;
	}
	if (upAxis == 0)
	{
		return;
	}
	std::string messageToSend;
	messageToSend += "InputMove " + std::to_string(m_clientId) + " " + std::to_string(upAxis);
	m_udpClient->SendMessageUDP(messageToSend);
}

void App::EventValidateTextCallback(std::string text)
{
	std::stringstream ss(text);
	std::string ip;
	int port;
	std::getline(ss, ip, ':');
	ss >> port;
	std::cout << "IP: " << ip << std::endl;
	std::cout << "Port: " << port << std::endl;
	m_udpClient->TryConnect(ip, port);
}


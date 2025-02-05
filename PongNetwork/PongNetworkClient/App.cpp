#include <iostream>
#include <sstream>
#include <string>

#include "Button.h"
#include "App.h"
#include "EventHandler.h"
#include "TextField.h"
#include "UDPClient.h"
#include "Ball.h"
#include "UserInterface.h"

using namespace std::placeholders;

sf::RenderWindow* App::Window;
sf::Font* App::MainFont = nullptr;

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
		else if (responseValue == 1)
		{
			SetErroText("The server is full");
		}
	}
	else if (messageType == "Paddle")
	{
		HandlePaddleMessage(buffer);
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
			if (player->ClientId == clientId)
			{
				player->score = newScore;
				m_userInterface->SetPlayerScore(clientId, newScore);
				std::cout << "Player " << player->ClientId << " score is " << player->score << std::endl;
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
	else if (messageType == "NewPlayer")
	{
		HandleNewPlayerMessage(buffer);
	}
}

void App::Init()
{
	MainFont = new sf::Font();
	if (!MainFont->openFromFile("Fonts/arial/arial.ttf"))
	{
		std::cout << "Failed to load font\n";
		return;
	}
	m_udpClient = new UDPClient();
	m_udpClient->Init();
	
	Window = new sf::RenderWindow(sf::VideoMode({ 1600, 900 }), "SFML works!", sf::Style::Titlebar | sf::Style::Close);
	Window->setFramerateLimit(60);
	
	m_addressTextField = new TextField();
	m_addressTextField->Init(sf::Vector2f(625,245), sf::Vector2f(350,75), "");
	m_eventValidateTextId = m_addressTextField->OnValidateText += [this](std::string text)
	{EventValidateTextCallback(text);};
	
	m_nameTextField = new TextField();
	m_nameTextField->Init(sf::Vector2f(625,145), sf::Vector2f(350,75), "");

	m_validateButton = new Button();
	m_validateButton->Init(sf::Vector2f(625,345), sf::Vector2f(350,75), "Validate");
	m_validateButton->OnClickEvent += [this](){OnValidateConnectionFields();};

	m_errorText = new sf::Text(*MainFont, "Error Text");
	m_errorText->setOrigin(sf::Vector2f(0.5f, 0.5f));

	auto center = m_errorText->getGlobalBounds().size / 2.f;
	auto localBounds = center + m_errorText->getLocalBounds().position;
	auto rounded = sf::Vector2f{ std::round(localBounds.x), std::round(localBounds.y) };
	m_errorText->setOrigin(rounded);
	
	m_errorText->setPosition({800.f, 500.f});
	
	m_userInterface = new UserInterface();
	m_userInterface->Init();
	
}

void App::Update()
{
	for (auto player: m_players)
	{
		player->Character->Move(player->InputMove);
	}
	if (m_twoPlayerJoined)
	{
		ball->Move();
		for (auto player : m_players)
		{
			ball->OnPaddleCollision(player->Character);
		}
		
		if (ball->GetPosition().x - (ball->GetShape().getRadius() * 2) <= 0)
		{
			playerWhoScored = 1;

			// Send which player scored to the server
			std::string messageToSend = "Score";
			messageToSend += " " + std::to_string(playerWhoScored);
			m_udpClient->SendMessageUDP(messageToSend);
		}

		if (ball->GetPosition().x >= GetWindowSize().x)
		{
			playerWhoScored = 0;

			// Send which player scored to the server
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
		Window->draw(*m_addressTextField);
		Window->draw(*m_nameTextField);
		Window->draw(*m_validateButton);
		Window->draw(*m_errorText);
	}
	else
	{
		for (auto player: m_players)
		{
			player->Character->Draw(Window);
		}

		if (m_twoPlayerJoined)
		{
			ball->Draw(Window);
		}
		Window->draw(*m_userInterface);

		// Dessiner le score � l'�cran
	}
	Window->display();
}

void App::HandleEvents()
{
	EventHandler::HandleEvent(Window);
}

void App::JoinGame()
{
	m_eventPlayerInputPressedId = EventHandler::OnKeyPressed += [this](const sf::Event::KeyPressed* event)
	{EventKeyPressedCallback(event);};
	m_eventPlayerInputReleasedId = EventHandler::OnKeyReleased += [this](const sf::Event::KeyReleased* event)
	{EventKeyReleasedCallback(event);};
	m_addressTextField->OnValidateText -= m_eventValidateTextId;

	
}

void App::HandlePaddleMessage(char messageBuffer[])
{
	char type[50];
	int clientId;
	float posX, posY;
	float upAxis;
	sscanf_s(messageBuffer, "%s %d %f %f %f", &type, (unsigned)_countof(type), &clientId, &posX, &posY, &upAxis);

	std::cout << "[PADDLE] Client: " << clientId
	<< ", Position: (" << posX << ", " << posY << ")"
	<< ", UpAxis: " << upAxis << std::endl;
	
	for (auto player: m_players)
	{
		if (player->ClientId == clientId)
		{
			player->Character->SetPosition(sf::Vector2f(posX, posY));
			player->InputMove = upAxis;
		}
	}
}

void App::HandleNewPlayerMessage(char messageBuffer[])
{
	char type[50];
	int clientId;
	float posX, posY;
	float upAxis;
	char name[100];
	sscanf_s(messageBuffer, "%s %d %f %f %f %s",
		&type, (unsigned)_countof(type),
		&clientId,
		&posX,
		&posY,
		&upAxis,
		&name, (unsigned)_countof(name));

	std::cout << "[New Plauyer] Client: " << clientId
	<< ", Position: (" << posX << ", " << posY << ")"
	<< ", UpAxis: " << upAxis 
	<< ", Name: " << name << std::endl ;

	Player* newPlayer = new Player();
	newPlayer->ClientId = clientId;
	newPlayer->Character =
		new Paddle(posX, posY, 20, 80, 10, sf::Vector2f(Window->getSize()));
	newPlayer->Name = name;
	if (clientId == m_clientId)
	{
		m_paddle = newPlayer->Character;
	}
	newPlayer->InputMove = upAxis;
	m_players.push_back(newPlayer);
	m_userInterface->AddPlayer(clientId, newPlayer->Name + " " + std::to_string(clientId), true);
}

void App::OnChangeUpAxis()
{
	std::string messageToSend;
	messageToSend +=
		"InputMove "
	+ std::to_string(m_clientId) + " "
	+ std::to_string(m_inputMove) + " "
	+ std::to_string(m_paddle->GetPosition().x) + " "
	+ std::to_string(m_paddle->GetPosition().y);
	m_udpClient->SendMessageUDP(messageToSend);
}

void App::SetErroText(const std::string& text)
{
	m_errorText->setString(text);
	auto center = m_errorText->getGlobalBounds().size / 2.f;
	auto localBounds = center + m_errorText->getLocalBounds().position;
	auto rounded = sf::Vector2f{ std::round(localBounds.x), std::round(localBounds.y) };
	m_errorText->setOrigin(rounded);

}

void App::OnValidateConnectionFields()
{
	auto address = m_addressTextField->GetText();
	auto name = m_nameTextField->GetText();
	if (name == "" || address == "")
	{
		SetErroText("You have to fill both fields");
		return;
	}
	std::stringstream ss(address);
	std::string ip;
	int port;
	std::getline(ss, ip, ':');
	ss >> port;
	m_udpClient->TryConnect(ip, port, name);
}

void App::EventKeyPressedCallback(const sf::Event::KeyPressed* event)
{
	float upAxis = 0;
	int speed = 1;
	if (event->scancode == sf::Keyboard::Scancode::Up)
	{
		upAxis-= 1 * speed;
	}
	if (event->scancode == sf::Keyboard::Scancode::Down)
	{
		upAxis+= 1 * speed;
	}

	if (upAxis == m_inputMove)
	{
		return;
	}
	m_inputMove = upAxis;
	OnChangeUpAxis();
}

void App::EventKeyReleasedCallback(const sf::Event::KeyReleased* event)
{
	float upAxis = m_inputMove;
	int speed = 1;
	if (event->scancode == sf::Keyboard::Scancode::Up)
	{
		upAxis+= 1 ;
	}
	if (event->scancode == sf::Keyboard::Scancode::Down)
	{
		upAxis-= 1 ;
	}
	if (upAxis == m_inputMove)
	{
		return;
	}
	m_inputMove = upAxis;
	OnChangeUpAxis();
}

void App::EventValidateTextCallback(std::string text)
{
	// std::stringstream ss(text);
	// std::string ip;
	// int port;
	// std::getline(ss, ip, ':');
	// ss >> port;
	// std::cout << "IP: " << ip << std::endl;
	// std::cout << "Port: " << port << std::endl;
	// m_udpClient->TryConnect(ip, port);
}


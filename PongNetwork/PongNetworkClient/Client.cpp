#include <SFML/Graphics.hpp>
#include <iostream>
#include "Client.h"
#include "Game.h"
#include "UDPClient.h"
#include "Menu.h"
#include "EventHandler.h"

sf::Font* Client::s_MainFont = nullptr;

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
}

void Client::Update(float deltaTime)
{
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
}

void Client::Run()
{
	Init();
	sf::Clock clock;
	while (m_window->isOpen())
	{
		sf::Time timeElapsed = clock.restart();
		HandleSfmlEvents();
		Update(timeElapsed.asSeconds());
		Draw();


	}
}

#include <iostream>
#include "Button.h"
#include "App.h"
#include "EventHandler.h"

using namespace std::placeholders;

sf::RenderWindow* App::Window;

void App::Run()
{
	Init();
	while (Window->isOpen())
	{
		HandleEvents();
		Update();
		Draw();
	}
}

sf::Vector2u App::GetWindowSize()
{
	return Window->getSize();
}

void App::Init()
{
	Window = new sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "SFML works!");
	m_baseShape = new sf::CircleShape(100.f);
	m_baseShape->setFillColor(sf::Color::Green);

	m_button = new Button();
	m_button->Init(sf::Vector2f(500,500), sf::Vector2f(100,50));
	m_button->OnClick += [this](){TestOnButtonClick();};
	
	auto id3 = EventHandler::OnKeyPressed += [this](const sf::Event::KeyPressed* event) {TestHandleInput(event);};
}

void App::Update()
{
}

void App::Draw()
{
	Window->clear();
	Window->draw(*m_baseShape);
	Window->draw(*m_button);
	Window->display();
}

void App::HandleEvents()
{
	EventHandler::HandleEvent(Window);
	// while (const std::optional event = m_window->pollEvent())
	// {
	// 	if (event->is<sf::Event::Closed>())
	// 		m_window->close();
	// }
}

void App::TestOnButtonClick()
{
	std::cout << "OnClickButton\n";
}

void App::TestHandleInput(const sf::Event::KeyPressed* event)
{
	std::cout << "Key pressed !\n";
}

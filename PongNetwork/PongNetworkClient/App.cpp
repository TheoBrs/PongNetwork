#include <iostream>
#include "App.h"

void App::Run()
{
	Init();
	while (m_window->isOpen())
	{
		HandleEvents();
		Update();
		Draw();
	}
}

void App::Init()
{
	m_window = new sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "SFML works!");
	m_baseShape = new sf::CircleShape(100.f);
	m_baseShape->setFillColor(sf::Color::Green);

	auto bind = std::bind(&App::TestCallback, this);
	auto bind2 = std::bind(&App::Test2Callback, this);

	m_callback += bind;
	m_callback += bind2;
	m_callback();
}

void App::Update()
{
}

void App::Draw()
{
	m_window->clear();
	m_window->draw(*m_baseShape);
	m_window->display();
}

void App::HandleEvents()
{
	while (const std::optional event = m_window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			m_window->close();
	}

}

void App::TestCallback()
{
	std::cout<< "Callback 1" <<std::endl;
}

void App::Test2Callback()
{
	std::cout<< "Callback 2" <<std::endl;
}

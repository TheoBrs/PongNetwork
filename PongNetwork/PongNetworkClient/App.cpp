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

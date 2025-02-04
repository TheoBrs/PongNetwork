#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "EventCallback.h"

class Button;

class App
{
public:
	void Run();

	sf::Vector2u GetWindowSize();
	static sf::RenderWindow* Window;
	
private:
	sf::CircleShape* m_baseShape;
	sf::RectangleShape* m_testButton;
	Button* m_button;

	void Init();
	void Update();
	void Draw();
	void HandleEvents();

	void TestOnButtonClick();
	void TestHandleInput(const sf::Event::KeyPressed* event);
};


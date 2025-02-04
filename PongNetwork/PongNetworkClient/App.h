#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "EventCallback.h"

class Button;
class TextField;
class App
{
public:
	void Run();

	sf::Vector2u GetWindowSize();
	static sf::RenderWindow* Window;
	
private:
	sf::CircleShape* m_baseShape;
	sf::RectangleShape* m_testButton;
	TextField* m_textField;

	void Init();
	void Update();
	void Draw();
	void HandleEvents();

	void TestOnButtonClick();
	void TestHandleInput(const sf::Event::KeyPressed* event);
};


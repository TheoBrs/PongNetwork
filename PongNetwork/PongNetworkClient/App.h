#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>



class App
{
public:
	void Run();

private:
	sf::RenderWindow* m_window;
	sf::CircleShape* m_baseShape;
	sf::RectangleShape* m_testButton;

	void Init();
	void Update();
	void Draw();
	void HandleEvents();
};


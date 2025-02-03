#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "EventCallback.h"

class App
{
public:
	void Run();

private:
	sf::RenderWindow* m_window;
	sf::CircleShape* m_baseShape;
	sf::RectangleShape* m_testButton;
	Utils::EventCallback<void()> m_callback;

	void Init();
	void Update();
	void Draw();
	void HandleEvents();

	void TestCallback();
	void Test2Callback();
};


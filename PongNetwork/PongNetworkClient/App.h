#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "EventCallback.h"

class Button;
class TextField;
class UDPClient;
class Paddle;

struct Player
{
	int ClientId = -1;
	Paddle* Character = nullptr;
};

class App
{
public:
	void Run();

	sf::Vector2u GetWindowSize();
	static sf::RenderWindow* Window;
	
private:
	sf::RectangleShape* m_testButton;
	TextField* m_textField;
	UDPClient* m_udpClient;
	std::vector<Player> m_players;
	
	int m_clientId = -1;
	int m_eventValidateTextId = -1;
	int m_eventplayerInputId = -1;

	bool m_isServerJoined = false;
	
	void HandleServerMessages();
	void Init();
	void Update();
	void Draw();
	void HandleEvents();
	void JoinGame();

	void HandlePadleMessage(char messageBuffer[]);
	
	void EventKeyPressedCallback(const sf::Event::KeyPressed* event);
	void EventValidateTextCallback(std::string text);
};


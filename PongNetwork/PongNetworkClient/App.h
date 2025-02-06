#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "EventCallback.h"

class Button;
class TextField;
class UDPClient;
class Paddle;
class Ball;
class UserInterface;

struct Player
{
	int ClientId = -1;
	Paddle* Character = nullptr;
	int score = 0;
	float InputMove = 0.f;
	std::string Name = "";
	bool isConnected = false;
};

class App
{
public:
	void Run();

	sf::Vector2u GetWindowSize();
	static sf::RenderWindow* Window;
	static sf::Font* MainFont;
	
private:
	sf::Text* m_errorText;
	sf::Text* m_adressText;
	sf::Text* m_nameText;

	TextField* m_addressTextField;
	TextField* m_nameTextField;

	Button* m_validateButton;

	
	UDPClient* m_udpClient;
	Ball* m_ball;
	std::string scoreText = "";
	UserInterface* m_userInterface;
	
	// 1 : Left | 2 : Right | 3 : Up | 4 : Down
	int playerWhoScored = 0;

	std::vector<Player*> m_players;


	Paddle* m_paddle = nullptr; 
	int m_clientId = -1;
	float m_inputMove = 0.0f;
	int m_eventValidateTextId = -1;
	int m_eventPlayerInputPressedId = -1;
	int m_eventPlayerInputReleasedId = -1;

	bool m_isServerJoined = false;
	bool m_twoPlayerJoined = false;
	
	void HandleServerMessages();
	void HandleEvents();
	void Init();
	void Update();
	void Draw();
	void JoinGame();

	void HandlePaddleMessage(char messageBuffer[]);
	void HandleNewPlayerMessage(char messageBuffer[]);
	
	void OnValidateConnectionFields();
	void SetErroText(const std::string& text);
	void EventValidateTextCallback(std::string text);
	
	void OnChangeUpAxis();
	void EventKeyPressedCallback(const sf::Event::KeyPressed* event);
	void EventKeyReleasedCallback(const sf::Event::KeyReleased* event);
};




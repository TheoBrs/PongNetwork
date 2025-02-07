#pragma once

enum PacketType
{
	ConnectionResponse,
	GameSettings,
	NewPlayer,
	Ball,
	Paddle,
	Score,
	IsGameRunning,
	Invalid
};

namespace Pong
{
	class Game;
}
namespace sf 
{
	class RenderWindow;
	class Font;
}

class UDPClient;
class Menu;

class Client
{
private:
	UDPClient* m_udpClient;
	Pong::Game* m_game;
	Menu* m_menu;
	sf::RenderWindow* m_window;

	bool m_isInGame;

	void Init();
	void Update(float deltaTime);
	void Draw();
	void HandleSfmlEvents();
	void Connection(std::string ip, int port, std::string name);

public:
	void Run();
	static sf::Font* s_MainFont;
};


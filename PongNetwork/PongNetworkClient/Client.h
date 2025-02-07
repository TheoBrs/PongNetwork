#pragma once
#include "UDPClient.h"
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

class Menu;
class PLayerController;
class UserInterface;

class Client
{
private:
	UDPClient* m_udpClient;
	Pong::Game* m_game;
	Menu* m_menu;
	sf::RenderWindow* m_window;
	PLayerController* m_playerController;
	UserInterface* m_userInterface;

	bool m_isInGame;
	bool m_isConnected;
	int m_clientId;

	PacketType ResolvePacketType(const std::string& input);

	void Init();
	void Update(float deltaTime);
	void Draw();
	void HandleSfmlEvents();
	void GetMessages();
	void HandleMessage(char(&buffer)[BUFFER_SIZE]);

	void Connection(std::string ip, int port, std::string name);
	void SendOnChangeInput(float input);

#pragma region HandleMessageMethods
	void HandleMessageConnectionResponse(char(&buffer)[BUFFER_SIZE]);
	void HandleMessageGameSettings(char(&buffer)[BUFFER_SIZE]);
	void HandleMessageNewPlayer(char(&buffer)[BUFFER_SIZE]);
	void HandleMessageBall(char(&buffer)[BUFFER_SIZE]);
	void HandleMessagePaddle(char(&buffer)[BUFFER_SIZE]);
	void HandleMessageScore(char(&buffer)[BUFFER_SIZE]);
	void HandleMessageIsGameRunning(char(&buffer)[BUFFER_SIZE]);
#pragma endregion


public:
	void Run();
	static sf::Font* s_MainFont;
};


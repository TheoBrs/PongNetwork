#pragma once

#include <unordered_map>
#include "ServerSettings.h"
#include "UDPServer.h"

#pragma region Forward Declaration

namespace Pong 
{
	class Game;
	class Paddle;
}

#pragma endregion



inline constexpr int TICK_TIME_MILLISECOND = 16;

enum PacketType
{
	ConnectionRequest,
	ConnectionResponse,
	InputChange,
	Disconnection,
	Invalid
};

class Server
{
private:
	UDPServer* m_udpServer;
	Pong::Game* m_game;
	std::unordered_map<int, bool> m_players;
	Pong::GameSettings* m_gameSettings;

	void Update(float deltaTime);
	void Init();
	PacketType ResolvePacketType(const std::string& input);
	void ProcessMessages(const BuffersToTreat& buffers);

	void ProcessPacketByType(
		const std::string& type, const sockaddr_in& address,const std::array<char, BUFFER_SIZE>& buffer
	);

	void HandleConnectionRequest(
		const sockaddr_in& address, const std::array<char, BUFFER_SIZE>& buffer);
	void HandleInputChange(
		const std::array<char, BUFFER_SIZE>& buffer);

	void SendBallCurrentState();
	void SendPaddleCurrentState(int clientId);
	void SendScore();
	void SendGameIsRunning();

	void SendGameState();
	void SendGameSettings(int clientID);

public:
	void Run();

};


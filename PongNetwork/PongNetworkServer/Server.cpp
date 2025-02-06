#include <SFML/Graphics.hpp>
#include "Server.h"
#include "UDPServer.h"
#include "Game.h"
#include "Settings.h"
#include "Paddle.h"
#include "Ball.h"

void Server::Update(float deltaTime)
{
	m_game->Update(deltaTime);
}

void Server::Init()
{
	m_udpServer = new UDPServer();
	m_udpServer->Init();

	m_game = new Pong::Game();
	Pong::GameSettings settings
	{
		SCREEN_SIZE,
		BALL_SETTINGS,
		PADDLE_SETTINGS_LEFT,
		PADDLE_SETTINGS_RIGHT
	};

	m_game->Init(settings);
	m_game->SetIsGameRunning(false);

	m_game->GetBall()->OnBounce += [this]() {SendBallCurrentState(); };
	m_game->OnAddScore += [this](bool isLeft) 
		{
			m_game->ResetForNewRound();
			m_game->SetIsGameRunning(false);
			SendGameState();
		};
}

PacketType Server::ResolvePacketType(const std::string& input)
{
	if (input == "ConnectionRequest") return ConnectionRequest;
	if (input == "ConnectionResponse") return ConnectionResponse;
	if (input == "InputChange") return InputChange;
	if (input == "Disconnection") return Disconnection;

	return PacketType::Invalid;
}



void Server::ProcessMessages(const BuffersToTreat& buffers)
{
	for (auto& [clientAddr, bufferList] : buffers)
	{
		for (auto buffer : bufferList) 
		{
			std::string message(std::begin(buffer), std::end(buffer));
			auto packetType = message.substr(0, message.find(' '));
			ProcessPacketByType(packetType, clientAddr,buffer);
		}

	}
}

void Server::ProcessPacketByType(
	const std::string& type, const sockaddr_in& address, const std::array<char, BUFFER_SIZE>& buffer)
{

	switch (ResolvePacketType(type))
	{
	case PacketType::ConnectionRequest:
		HandleConnectionRequest(address, buffer);
		break;
	case PacketType::InputChange:
		HandleInputChange(buffer);
		break;
	default:
		break;
	}
}

void Server::HandleConnectionRequest(
	const sockaddr_in& address, const std::array<char, BUFFER_SIZE>& buffer)
{
	int nbClient = m_udpServer->GetClients().size();
	if (nbClient >= 2)
	{
		return;
	}
	int clientId = nbClient;

	char bufferC[BUFFER_SIZE];
	std::copy(std::begin(buffer), std::end(buffer), std::begin(bufferC));
	char type[50];
	char name[100];
	sscanf_s(bufferC, "%s %s", &type, (unsigned)_countof(type), &name, (unsigned)_countof(name));
	m_udpServer->AddClient(clientId, address, name);

	bool isLeft = m_players.size() > 0;
	m_players[clientId] = m_game->GetPaddle(isLeft);

	std::string messageConnectionResponse = "ConnectionResponse 0 " + std::to_string(clientId);
	m_udpServer->AddMessageUDP(clientId, messageConnectionResponse);

	std::string messageNewPlayer = "NewPlayer " + std::to_string(clientId) + " " + std::to_string(isLeft ? 1 : 0);
	m_udpServer->AddMessageUDPAll(messageConnectionResponse);

	
}

void Server::HandleInputChange(const std::array<char, BUFFER_SIZE>& buffer)
{
	char bufferC[BUFFER_SIZE];
	std::copy(std::begin(buffer), std::end(buffer), std::begin(bufferC));
	char type[50];
	int clientId;
	float upAxis;
	sscanf_s(bufferC, "%s %d %f", &type, (unsigned)_countof(type), &clientId, &upAxis);
	
	upAxis = std::clamp<float>(-1.f, 1.f, upAxis);
	m_players[clientId]->SetDirection(upAxis);
	SendPaddleCurrentState(clientId);
}

void Server::SendBallCurrentState()
{
	Pong::Ball* ball = m_game->GetBall();
	std::string messageToSend =
		"Ball " +
		std::to_string(ball->GetDirection().x) + " " +
		std::to_string(ball->GetDirection().y) + " " +
		std::to_string(ball->GetPosition().x) + " " +
		std::to_string(ball->GetPosition().y) + " ";
	m_udpServer->AddMessageUDPAll(messageToSend);
}

void Server::SendPaddleCurrentState(int clientId)
{
	float upAxis = m_players[clientId]->GetDirection();
	std::string messageToSend =
		"Paddle " +
		std::to_string(clientId) + " " +
		std::to_string(upAxis) + " " +
		std::to_string(m_players[clientId]->GetPosition().x) + " " +
		std::to_string(m_players[clientId]->GetPosition().y) + " "
		;
	m_udpServer->AddMessageUDPAll(messageToSend);
}

void Server::SendScore()
{
	int scoreLeft = m_game->GetScore(true);
	int scoreRight = m_game->GetScore(false);
	std::string messageToSend =
		"Score " +
		std::to_string(scoreLeft) + " " +
		std::to_string(scoreRight);
	m_udpServer->AddMessageUDPAll(messageToSend);
}

void Server::SendGameIsRunning()
{
	std::string messageToSend =
		"IsGameRunning " +
		std::to_string(m_game->GetIsGameRunning());
}

void Server::SendGameState()
{
	SendGameIsRunning();
	SendBallCurrentState();
	for (auto [clientId, player] : m_players) 
	{
		SendPaddleCurrentState(clientId);
	}
	SendScore();
}


void Server::Run()
{
	sf::Clock clock;
	Init();
	while (true) {
		BuffersToTreat buffers;
		m_udpServer->ReceiveMessages(&buffers);
		ProcessMessages(buffers);
		sf::Time elapsedTime = clock.getElapsedTime();
		if (elapsedTime.asMilliseconds() >= TICK_TIME_MILLISECOND)
		{
			Update(elapsedTime.asSeconds());
			clock.restart();
		}
		m_udpServer->SendMessages();
	}
	m_udpServer->UnInit();
}

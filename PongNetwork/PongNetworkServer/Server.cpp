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
	if (m_isGameLaunched)
	{
		m_timeElapsedBeforeLastReset += deltaTime;
		if (m_timeElapsedBeforeLastReset >= TIME_BETWEEN_ROUNDS && !m_game->GetIsGameRunning())
		{
			m_game->SetIsGameRunning(true);
			SendGameIsRunning();
		}
	}
}

void Server::Init()
{
	m_udpServer = new UDPServer();
	m_udpServer->Init();

	m_game = new Pong::Game();
	m_gameSettings = new Pong::GameSettings();
	*m_gameSettings =
	{
		SCREEN_SIZE,
		BALL_SETTINGS,
		PADDLE_SETTINGS_LEFT,
		PADDLE_SETTINGS_RIGHT
	};
	m_isGameLaunched = false;
	m_game->Init(*m_gameSettings);
	m_game->SetIsGameRunning(false);

	m_game->GetBall()->OnBounce += [this]() {SendBallCurrentState(); };
	m_game->OnAddScore += [this](bool isLeft)
		{
			m_timeElapsedBeforeLastReset = 0.f;
			m_game->ResetForNewRound();
			m_game->SetIsGameRunning(false);
			SendGameState();
		};
	m_game->GetBall()->SetDirection({ 4.3f, 3.7f });


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
	for (auto& [clientAddr, buffer] : buffers)
	{

		std::string message(std::begin(buffer), std::end(buffer));
		auto packetType = message.substr(0, message.find(' '));
		ProcessPacketByType(packetType, clientAddr,buffer);
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

	bool isLeft = m_players.size() == 0;
	m_players[clientId] = isLeft;

	std::string messageConnectionResponse = "ConnectionResponse 0 " + std::to_string(clientId);
	m_udpServer->AddMessageUDP(clientId, messageConnectionResponse);
	SendGameSettings(clientId);
	std::string nameString = name;

	std::string messageNewPlayer = "NewPlayer " + nameString + " " + std::to_string(isLeft ? 1 : 0);
	m_udpServer->AddMessageUDPAll(messageNewPlayer);
	for ( auto [id, isLeftPlayer] : m_players )
	{
		if (id != clientId) 
		{
			messageNewPlayer = "NewPlayer " + nameString + " " + std::to_string(isLeftPlayer ? 1 : 0);
			m_udpServer->AddMessageUDP(clientId, messageNewPlayer);
		}
	}

	if (m_players.size() >= 2)
	{
		m_game->SetIsGameRunning(true);
		SendGameState();
		m_isGameLaunched = true;
	}
}

void Server::HandleInputChange(const std::array<char, BUFFER_SIZE>& buffer)
{
	char bufferC[BUFFER_SIZE];
	std::copy(std::begin(buffer), std::end(buffer), std::begin(bufferC));
	char type[50];
	int clientId;
	float upAxis;
	sscanf_s(bufferC, "%s %d %f", &type, (unsigned)_countof(type), &clientId, &upAxis);
	
	upAxis = std::clamp<float>(upAxis, -1.f, 1.f);
	Pong::Paddle* paddle = m_game->GetPaddle(m_players[clientId]);
	paddle->SetDirection(upAxis);
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
	Pong::Paddle* paddle = m_game->GetPaddle(m_players[clientId]);

	float upAxis = paddle->GetDirection();
	bool isLeft = m_players[clientId];
	std::string messageToSend =
		"Paddle " +
		std::to_string(isLeft ? 1 : 0) + " " +
		std::to_string(upAxis) + " " +
		std::to_string(paddle->GetPosition().x) + " " +
		std::to_string(paddle->GetPosition().y) + " "
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
		std::to_string(m_game->GetIsGameRunning() ? 1 : 0);
	m_udpServer->AddMessageUDPAll(messageToSend);
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

void Server::SendGameSettings(int clientID)
{
	std::string paddleLeftSettings, paddleRightSettings, ballSettings;

	paddleLeftSettings =
		std::to_string(m_gameSettings->PaddleLeft.BasePosition.x) + " " +
		std::to_string(m_gameSettings->PaddleLeft.BasePosition.y) + " " +
		std::to_string(m_gameSettings->PaddleLeft.BasPaddleSpeed) + " " +
		std::to_string(m_gameSettings->PaddleLeft.Width) + " " +
		std::to_string(m_gameSettings->PaddleLeft.Height);

	paddleRightSettings =
		std::to_string(m_gameSettings->PaddleRight.BasePosition.x) + " " +
		std::to_string(m_gameSettings->PaddleRight.BasePosition.y) + " " +
		std::to_string(m_gameSettings->PaddleRight.BasPaddleSpeed) + " " +
		std::to_string(m_gameSettings->PaddleRight.Width) + " " +
		std::to_string(m_gameSettings->PaddleRight.Height);

	ballSettings =
		std::to_string(m_gameSettings->Ball.BasePosition.x) + " " +
		std::to_string(m_gameSettings->Ball.BasePosition.y) + " " +
		std::to_string(m_gameSettings->Ball.BaseBallSpeed) + " " +
		std::to_string(m_gameSettings->Ball.Radius);

	std::string messageToSend =
		"GameSettings " +
		std::to_string(m_gameSettings->ScreeSize.x) + " " +
		std::to_string(m_gameSettings->ScreeSize.y) + " " +
		paddleLeftSettings + " " +
		paddleRightSettings + " " +
		ballSettings;
	
	m_udpServer->AddMessageUDP(clientID, messageToSend);
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

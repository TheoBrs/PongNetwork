#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Paddle.h"
#include "Ball.h"

void Pong::Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_paddleLeft);
	target.draw(*m_paddleRight);
	target.draw(*m_ball);
}

void Pong::Game::Init(const GameSettings& settings)
{
	m_settings = settings;
	m_ball = new Ball(m_settings.Ball, {0,0}, m_settings.ScreeSize);
	m_paddleLeft = new Paddle(m_settings.PaddleLeft, m_settings.ScreeSize);
	m_paddleRight = new Paddle(m_settings.PaddleRight, m_settings.ScreeSize);
}

void Pong::Game::Update(float deltaTime)
{
	if (!m_isGameRunning) 
	{
		return;
	}

	m_paddleLeft->Move(deltaTime);
	m_paddleRight->Move(deltaTime);
	m_ball->Move(deltaTime);

	m_ball->OnPaddleCollision(m_paddleLeft);
	m_ball->OnPaddleCollision(m_paddleRight);

	if (m_ball->GetPosition().x - (m_ball->GetShape()->getRadius() * 2) <= 0)
	{
		AddScore(false);
	}
	if (m_ball->GetPosition().x >= m_settings.ScreeSize.x)
	{
		AddScore(true);
	}
}

void Pong::Game::ResetForNewRound()
{
	sf::Vector2f posLeft(m_settings.PaddleLeft.BasePosition);
	sf::Vector2f posRight(m_settings.PaddleLeft.BasePosition);
	sf::Vector2f posBall(m_settings.Ball.BasePosition);
	m_paddleLeft->SetPosition(posLeft);
	m_paddleRight->SetPosition(posRight);
	m_ball->GetShape()->setPosition(posBall);
}

void Pong::Game::AddScore(bool isLeft)
{
	isLeft ? m_scoreLeft += 1 : m_scoreRight += 1;
	OnAddScore(isLeft);
}

int Pong::Game::GetScore(bool isLeft)
{
	return isLeft ? m_scoreLeft : m_scoreRight;
}

void Pong::Game::SetIsGameRunning(bool isGameRunning)
{
	m_isGameRunning = isGameRunning;
}

Pong::Ball* Pong::Game::GetBall()
{
	return m_ball;
}

Pong::Paddle* Pong::Game::GetPaddle(bool isLeft)
{
	return isLeft ? m_paddleLeft : m_paddleRight;
}

#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Ball.h"
#include "Paddle.h"


Pong::Ball::Ball(float x, float y, float radius, sf::Vector2f direction, sf::Vector2f screenSize)
{
	this->m_direction = direction;
	this->m_screenSize = screenSize;

	m_shape->setRadius(radius);
	m_shape->setFillColor(sf::Color::White);
	m_shape->setPosition(sf::Vector2f(x, y));
}

Pong::Ball::Ball(const BallSettings& settings, sf::Vector2f direction, sf::Vector2f screenSize) 
{
	this->m_direction = direction;
	this->m_screenSize = screenSize;
	this->m_speed = settings.BaseBallSpeed;
	m_shape = new sf::CircleShape(settings.Radius);
	m_shape->setFillColor(sf::Color::White);
	m_shape->setPosition(settings.BasePosition);
}

void Pong::Ball::Move(float deltaTime)
{
	m_shape->move(m_direction * deltaTime * m_speed);

	OnWallCollision();
}

void Pong::Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_shape, states);

}


void Pong::Ball::OnWallCollision()
{
	/*
	// Left collision
	if (m_shape->getPosition().x <= 0)
	{
		m_shape->setPosition(sf::Vector2f(0, m_shape->getPosition().y));
		m_direction.x = -m_direction.x;
	}

	// Right collision
	if (m_shape->getPosition().x + (m_shape->getRadius() * 2) >= screenSize.x)
	{
		m_shape->setPosition(sf::Vector2f(screenSize.x - (m_shape->getRadius() * 2), m_shape->getPosition().y));
		m_direction.x = -m_direction.x;
	}
	*/

	// Up collision
	if (m_shape->getPosition().y <= 0)
	{
		m_shape->setPosition(sf::Vector2f(m_shape->getPosition().x, 0));
		m_direction.y = -m_direction.y;
	}

	// Down collision
	if (m_shape->getPosition().y + (m_shape->getRadius() * 2) >= m_screenSize.y)
	{
		m_shape->setPosition(
			sf::Vector2f(m_shape->getPosition().x, m_screenSize.y - (m_shape->getRadius() * 2)));
		m_direction.y = -m_direction.y;
	}
}
void Pong::Ball::OnPaddleCollision(Paddle* paddle)
{
	if (m_shape->getGlobalBounds().findIntersection(paddle->GetShape()->getGlobalBounds()))
	{
		std::string side = CheckSideOfCollision(paddle->GetShape());

		Bounce(paddle, side);
	}
}


// Check the side of the paddle the ball hit
std::string Pong::Ball::CheckSideOfCollision(sf::RectangleShape* paddleRect)
{
	// Calculates which side was the paddle touched
	float deltaLeft = 
		std::abs(paddleRect->getPosition().x - m_shape->getPosition().x - (m_shape->getRadius() * 2));
	float deltaRight = 
		std::abs(paddleRect->getPosition().x + paddleRect->getSize().x - m_shape->getPosition().x);
	float deltaTop = 
		std::abs((paddleRect->getPosition().y - m_shape->getPosition().y) - (m_shape->getRadius() * 2));
	float deltaBottom = 
		std::abs(paddleRect->getPosition().y + paddleRect->getSize().y - m_shape->getPosition().y);

	if (deltaLeft < deltaTop && deltaLeft < deltaRight && deltaLeft < deltaBottom) {
		// Left collision
		return "left";
	}
	else if (deltaRight < deltaTop && deltaRight < deltaLeft && deltaRight < deltaBottom) {
		// Right collision
		return "right";
	}
	else if (deltaTop < deltaLeft && deltaTop < deltaRight && deltaTop < deltaBottom) {
		// Up collision
		return "top";
	}
	else {
		// Down collision
		return "bottom";
	}
}

// Calculate new direction and velocity based of the side of the paddle the ball hit
void Pong::Ball::Bounce(Paddle* paddle, std::string side)
{
	float normalX = 0.0f, normalY = 0.0f;

	if (side == "left") {
		normalX = -1.0f;
	}
	else if (side == "right") {
		normalX = 1.0f;
	}
	else if (side == "top") {
		normalY = -1.0f;
	}
	else if (side == "bottom") {
		normalY = 1.0f;
	}

	// Calculate dot product
	float dotProduct = m_direction.x * normalX + m_direction.y * normalY;

	// Calculate reflection vectors
	float reflectX = m_direction.x - 2 * dotProduct * normalX;
	float reflectY = m_direction.y - 2 * dotProduct * normalY;

	// Calculate initial speed and new magnitude to normalize reflection vectors
	float initialSpeed = std::sqrt(m_direction.x * m_direction.x + m_direction.y * m_direction.y);
	float magnitude = std::sqrt(reflectX * reflectX + reflectY * reflectY);

	// Calculate new directions with the new values
	m_direction.x = (reflectX / magnitude) * initialSpeed;
	m_direction.y = (reflectY / magnitude) * initialSpeed;

	if (side == "left") {
		m_shape->setPosition(sf::Vector2f(paddle->GetShape()->getPosition().x - m_shape->getRadius() * 2, m_shape->getPosition().y));
	}
	else if (side == "right") {
		m_shape->setPosition(sf::Vector2f(paddle->GetShape()->getPosition().x + paddle->GetShape()->getSize().x, m_shape->getPosition().y));
	}
	else if (side == "top") {
		m_shape->setPosition(sf::Vector2f(m_shape->getPosition().x, paddle->GetShape()->getPosition().y - m_shape->getRadius() * 2));
	}
	else if (side == "bottom") {
		m_shape->setPosition(sf::Vector2f(m_shape->getPosition().x, paddle->GetShape()->getPosition().y + paddle->GetShape()->getSize().y));
	}

	if (side == "top" || side == "bottom") {
		m_direction.x += (m_shape->getPosition().x + m_shape->getRadius() - paddle->GetShape()->getPosition().x + (paddle->GetShape()->getSize().x / 2)) * 0.005f;
	}
	else if (side == "left" || side == "right") {
		m_direction.y += (m_shape->getPosition().y + m_shape->getRadius() - paddle->GetShape()->getPosition().y + (paddle->GetShape()->getSize().y / 2)) * 0.005f;
	}
	OnBounce();
}

sf::CircleShape* Pong::Ball::GetShape() const
{
	return m_shape;
}

sf::Vector2f Pong::Ball::GetPosition() const
{
	return m_shape->getPosition();
}

sf::Vector2f Pong::Ball::GetDirection() const
{
	return m_direction;
}
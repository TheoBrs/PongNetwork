#include <SFML/Graphics.hpp>
#include "Paddle.h"

void Pong::Paddle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(*m_shape, states);
}

Pong::Paddle::Paddle(const PaddleSettings& settings, sf::Vector2f screenSize)
{
    this->m_screenSize = screenSize;
    this->m_speed = settings.BasPaddleSpeed;
    m_shape = new sf::RectangleShape(sf::Vector2f(settings.Width, settings.Height));
    m_shape->setFillColor(sf::Color::White);
    m_shape->setPosition(settings.BasePosition);

}


Pong::Paddle::Paddle(float x, float y, float width, float height, float speed, sf::Vector2f screenSize)
{
    this->m_speed = speed;
    this->m_screenSize = screenSize;

    m_shape->setSize(sf::Vector2f(width, height));
    m_shape->setFillColor(sf::Color::White);
    m_shape->setPosition(sf::Vector2f(x, y));
}

void Pong::Paddle::Move(float deltaTime)
{
    sf::Vector2f startPosition = m_shape->getPosition();
    m_shape->move({ 0, m_directionY * m_speed * deltaTime});

    if (m_shape->getPosition().y < 0 && m_shape->getPosition().y + m_shape->getSize().y > m_screenSize.y)
    {
        m_shape->setPosition(startPosition);
    }
}

void Pong::Paddle::SetPosition(const sf::Vector2f& newPosition)
{
    m_shape->setPosition(newPosition);
}

void Pong::Paddle::SetDirection(float value)
{
    m_directionY = std::clamp<float>(-1.f, 1.f, value);
}

sf::RectangleShape* Pong::Paddle::GetShape() const
{
    return m_shape;
}

sf::Vector2f Pong::Paddle::GetPosition() const
{
    return m_shape->getPosition();
}

float Pong::Paddle::GetDirection() const
{
    return m_directionY;
}

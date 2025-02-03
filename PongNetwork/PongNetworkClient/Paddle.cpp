#include "Paddle.h"

Paddle::Paddle(float x, float y, float width, float height, float speed, sf::Vector2f screenSize) 
{
    this->speed = speed;
    this->screenSize = screenSize;

    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(sf::Color::White);
    shape.setPosition(sf::Vector2f(x, y));
}

void Paddle::Move(int direction)
{
    if (shape.getPosition().y < 0 && shape.getPosition().y + shape.getSize().y > screenSize.y)
    {
        shape.move(sf::Vector2f(0, direction * speed));
    }
}

void Paddle::Draw(sf::RenderWindow* window)
{
    window->draw(shape);
}

sf::RectangleShape Paddle::GetShape() const 
{
    return shape;
}

sf::Vector2f Paddle::GetPosition() const 
{
    return shape.getPosition();
}
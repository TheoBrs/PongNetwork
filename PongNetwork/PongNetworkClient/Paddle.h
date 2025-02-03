#pragma once

#include <SFML/Graphics.hpp>

class Paddle 
{
private:
    sf::RectangleShape shape;
    float speed;
    sf::Vector2f screenSize;

public:
    Paddle(float x, float y, float width, float height, float speed, sf::Vector2f screenSize);

    void Move(int direction);
    void Draw(sf::RenderWindow* window);

    sf::RectangleShape GetShape() const;
    sf::Vector2f GetPosition() const;
};
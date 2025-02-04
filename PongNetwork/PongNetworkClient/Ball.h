#pragma once

#include <SFML/Graphics.hpp>
#include "Paddle.h"

class Ball
{
private:
    sf::CircleShape shape;
    sf::Vector2f direction;
    sf::Vector2f screenSize;

public:
    Ball(float x, float y, float radius, sf::Vector2f direction, sf::Vector2f screenSize);

    void Move();
    void Draw(sf::RenderWindow* window);

    void OnWallCollision();
    void OnPaddleCollision(Paddle* paddle);
    std::string CheckSideOfCollision(sf::RectangleShape rect);
    void Bounce(Paddle* paddle, std::string side);

    sf::CircleShape GetShape() const;
    sf::Vector2f GetPosition() const;
    sf::Vector2f GetDirection() const;
};


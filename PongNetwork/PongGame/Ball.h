#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <string>
#include "Settings.h"
#include "EventCallback.h"

namespace sf
{
    class CircleShape;
    class RectangleShape;
}

namespace Pong
{
    class Paddle;

	class Ball : public sf::Drawable
	{
    private:
        sf::CircleShape* m_shape;
        sf::Vector2f m_direction;
        sf::Vector2f m_screenSize;
        float m_speed;
	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        Ball(const BallSettings& settings, sf::Vector2f direction, sf::Vector2f screenSize);
        Ball(float x, float y, float radius, sf::Vector2f direction, sf::Vector2f screenSize);

        void Move(float deltaTime);
        void OnWallCollision();
        void OnPaddleCollision(Paddle* paddle);
        std::string CheckSideOfCollision(sf::RectangleShape* rect);
        void Bounce(Paddle* paddle, std::string side);

        sf::CircleShape* GetShape() const;
        sf::Vector2f GetPosition() const;
        sf::Vector2f GetDirection() const;

        Utils::EventCallback<void> OnBounce;
	};

}
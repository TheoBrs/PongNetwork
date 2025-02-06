#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "Settings.h"

namespace sf 
{
	class RectangleShape;
}

namespace Pong
{


	class Paddle :public sf::Drawable
	{

	private:
		sf::RectangleShape* m_shape;
		float m_speed;
		sf::Vector2f m_screenSize;
		float m_directionY;
	protected:
		// Hérité via Drawable
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		Paddle(const PaddleSettings& settings, sf::Vector2f screenSize);
		Paddle(float x, float y, float width, float height, float speed, sf::Vector2f screenSize);

		void Move(float deltaTime);
		void SetPosition(const sf::Vector2f& newPosition);

		void SetDirection(float value);

		sf::RectangleShape* GetShape() const;
		sf::Vector2f GetPosition() const;
		float GetDirection() const;
	};

}

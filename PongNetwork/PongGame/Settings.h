#pragma once
#include <SFML/System/Vector2.hpp>

namespace Pong 
{
	struct BallSettings 
	{
	public:
		sf::Vector2f BasePosition;
		float BaseBallSpeed;
		float Radius;
	};

	struct PaddleSettings
	{
	public:
		sf::Vector2f BasePosition;
		float BasPaddleSpeed;
		float Width;
		float Height;
	};

	struct GameSettings
	{
	public:
		sf::Vector2f ScreeSize;
		BallSettings Ball;
		PaddleSettings PaddleLeft;
		PaddleSettings PaddleRight;
	};


}


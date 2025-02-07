#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "Settings.h"
#include "EventCallback.h"

namespace Pong 
{
	class Paddle;
	class Ball;

	class Game : public sf::Drawable
	{
		

	private:
		Ball* m_ball;
		Paddle* m_paddleLeft;
		Paddle* m_paddleRight;
		GameSettings m_settings;

		int m_scoreLeft;
		int m_scoreRight;
		bool m_isGameRunning;

	protected:
		// Hérité via Drawable
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	public:
		void Init(const GameSettings& settings);
		void Update(float deltaTime);
		void ResetForNewRound();

		void AddScore(bool isLeft);
		int GetScore(bool isLeft);
		void SetScore(int scoreLeft, int scoreRight);

		void SetIsGameRunning(bool isGameRunning);
		bool GetIsGameRunning();

		Ball* GetBall();
		Paddle* GetPaddle(bool isLeft);


		// The boolean is to here to know who's gain point
		Utils::EventCallback<void, bool> OnAddScore;

	};

}


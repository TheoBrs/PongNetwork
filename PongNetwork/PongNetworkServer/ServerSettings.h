#pragma once
#include "Settings.h"
#define BUFFER_SIZE 1024
#define PORT 54000

inline constexpr sf::Vector2f SCREEN_SIZE = { 1600, 900 };
inline constexpr Pong::BallSettings  BALL_SETTINGS = {
	{800.f, 450.f},
	5,
	10};

inline constexpr Pong::PaddleSettings PADDLE_SETTINGS_LEFT =
{
	{100, 400},
	10,
	80,
	10
};

inline constexpr Pong::PaddleSettings PADDLE_SETTINGS_RIGHT =
{
	{1500, 400},
	10,
	80,
	10
};



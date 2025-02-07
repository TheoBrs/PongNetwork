#pragma once
#include "Settings.h"
#define BUFFER_SIZE 1024
#define PORT 54000

inline constexpr sf::Vector2f SCREEN_SIZE = { 1600, 900 };
inline constexpr Pong::BallSettings  BALL_SETTINGS = {
	//Start Position
	{800.f, 450.f},
	//Speed
	50,
	//Radius
	10};

inline constexpr Pong::PaddleSettings PADDLE_SETTINGS_LEFT =
{
	//Start Position
	{100, 400},
	//Speed
	500,
	//Width
	10,
	//Height
	80
};

inline constexpr Pong::PaddleSettings PADDLE_SETTINGS_RIGHT =
{
	//Start Position
	{1500, 400},
	//Speed
	500,
	//Width
	10,
	//Height
	80
};



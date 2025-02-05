#include "Ball.h"

Ball::Ball(float x, float y, float radius, sf::Vector2f direction, sf::Vector2f screenSize)
{
	this->direction = direction;
	this->screenSize = screenSize;

	shape.setRadius(radius);
	shape.setFillColor(sf::Color::White);
	shape.setPosition(sf::Vector2f(x, y));
}

void Ball::Move()
{
	shape.move(direction);

	OnWallCollision();
}

void Ball::Draw(sf::RenderWindow* window)
{
	window->draw(shape);
}

void Ball::OnWallCollision()
{
	/*
	// Left collision
	if (shape.getPosition().x <= 0)
	{
		shape.setPosition(sf::Vector2f(0, shape.getPosition().y));
		direction.x = -direction.x;
	}

	// Right collision
	if (shape.getPosition().x + (shape.getRadius() * 2) >= screenSize.x)
	{
		shape.setPosition(sf::Vector2f(screenSize.x - (shape.getRadius() * 2), shape.getPosition().y));
		direction.x = -direction.x;
	}
	*/

	// Up collision
	if (shape.getPosition().y <= 0)
	{
		shape.setPosition(sf::Vector2f(shape.getPosition().x, 0));
		direction.y = -direction.y;
	}

	// Down collision
	if (shape.getPosition().y + (shape.getRadius() * 2) >= screenSize.y)
	{
		shape.setPosition(sf::Vector2f(shape.getPosition().x, screenSize.y - (shape.getRadius() * 2)));
		direction.y = -direction.y;
	}
}

void Ball::OnPaddleCollision(Paddle* paddle)
{
	if (shape.getGlobalBounds().findIntersection(paddle->GetShape().getGlobalBounds()))
	{
		std::string side = CheckSideOfCollision(paddle->GetShape());

		Bounce(paddle, side);
	}
}

// Check the side of the paddle the ball hit
std::string Ball::CheckSideOfCollision(sf::RectangleShape paddleRect)
{
	// Calculates which side was the paddle touched
	float deltaLeft = std::abs(paddleRect.getPosition().x - shape.getPosition().x - (shape.getRadius() * 2));
	float deltaRight = std::abs(paddleRect.getPosition().x + paddleRect.getSize().x - shape.getPosition().x);
	float deltaTop = std::abs((paddleRect.getPosition().y - shape.getPosition().y) - (shape.getRadius() * 2));
	float deltaBottom = std::abs(paddleRect.getPosition().y + paddleRect.getSize().y - shape.getPosition().y);

	if (deltaLeft < deltaTop && deltaLeft < deltaRight && deltaLeft < deltaBottom) {
		// Left collision
		return "left";
	}
	else if (deltaRight < deltaTop && deltaRight < deltaLeft && deltaRight < deltaBottom) {
		// Right collision
		return "right";
	}
	else if (deltaTop < deltaLeft && deltaTop < deltaRight && deltaTop < deltaBottom) {
		// Up collision
		return "top";
	}
	else {
		// Down collision
		return "bottom";
	}
}

// Calculate new direction and velocity based of the side of the paddle the ball hit
void Ball::Bounce(Paddle* paddle, std::string side)
{
	float normalX = 0.0f, normalY = 0.0f;

	if (side == "left") {
		normalX = -1.0f;
	}
	else if (side == "right") {
		normalX = 1.0f;
	}
	else if (side == "top") {
		normalY = -1.0f;
	}
	else if (side == "bottom") {
		normalY = 1.0f;
	}

	// Calculate dot product
	float dotProduct = direction.x * normalX + direction.y * normalY;

	// Calculate reflection vectors
	float reflectX = direction.x - 2 * dotProduct * normalX;
	float reflectY = direction.y - 2 * dotProduct * normalY;

	// Calculate initial speed and new magnitude to normalize reflection vectors
	float initialSpeed = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	float magnitude = std::sqrt(reflectX * reflectX + reflectY * reflectY);

	// Calculate new directions with the new values
	direction.x = (reflectX / magnitude) * initialSpeed;
	direction.y = (reflectY / magnitude) * initialSpeed;

	if (side == "left") {
		shape.setPosition(sf::Vector2f(paddle->GetShape().getPosition().x - shape.getRadius() * 2, shape.getPosition().y));
	}
	else if (side == "right") {
		shape.setPosition(sf::Vector2f(paddle->GetShape().getPosition().x + paddle->GetShape().getSize().x, shape.getPosition().y));
	}
	else if (side == "top") {
		shape.setPosition(sf::Vector2f(shape.getPosition().x, paddle->GetShape().getPosition().y - shape.getRadius() * 2));
	}
	else if (side == "bottom") {
		shape.setPosition(sf::Vector2f(shape.getPosition().x, paddle->GetShape().getPosition().y + paddle->GetShape().getSize().y));
	}

	if (side == "top" || side == "bottom") {
		direction.x += (shape.getPosition().x + shape.getRadius() - paddle->GetShape().getPosition().x + (paddle->GetShape().getSize().x / 2)) * 0.005f;
	}
	else if (side == "left" || side == "right") {
		direction.y += (shape.getPosition().y + shape.getRadius() - paddle->GetShape().getPosition().y + (paddle->GetShape().getSize().y / 2)) * 0.005f;
	}
}

sf::CircleShape Ball::GetShape() const
{
	return shape;
}

sf::Vector2f Ball::GetPosition() const
{
	return shape.getPosition();
}

sf::Vector2f Ball::GetDirection() const
{
	return direction;
}

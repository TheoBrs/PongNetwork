#include "PLayerController.h"
#include "EventHandler.h"

void PLayerController::EventKeyPressedCallback(const sf::Event::KeyPressed* event)
{
	if (!m_isActive) 
	{
		return;
	}

	if (event->scancode == sf::Keyboard::Scancode::Up)
	{
		m_isUpPressed = true;
	}
	if (event->scancode == sf::Keyboard::Scancode::Down)
	{
		m_isDowPressed = true;
	}
	float upAxis = (m_isUpPressed ? -1.f : 0.f) + (m_isDowPressed ? 1.f : 0.f);
	if (upAxis != m_upAxis)
	{
		m_upAxis = upAxis;
		OnChangeInput(m_upAxis);
	}
}

void PLayerController::EventKeyReleasedCallback(const sf::Event::KeyReleased* event)
{
	if (!m_isActive)
	{
		return;
	}

	if (event->scancode == sf::Keyboard::Scancode::Up)
	{
		m_isUpPressed = false;
	}
	if (event->scancode == sf::Keyboard::Scancode::Down)
	{
		m_isDowPressed = false;
	}
	float upAxis = (m_isUpPressed ? -1.f : 0.f) + (m_isDowPressed ? 1.f : 0.f);
	if (upAxis != m_upAxis)
	{
		m_upAxis = upAxis;
		OnChangeInput(m_upAxis);
	}
}

void PLayerController::Init()
{
	EventHandler::OnKeyPressed += [this](const sf::Event::KeyPressed* event) 
		{
			EventKeyPressedCallback(event);
		};
	EventHandler::OnKeyReleased += [this](const sf::Event::KeyReleased* event)
		{
			EventKeyReleasedCallback(event);
		};
	m_isActive = false;
	m_isUpPressed = false;
	m_isUpPressed = false;
	m_upAxis = 0.f;
}

void PLayerController::SetActive(bool value)
{
	m_isActive = value;
}

bool PLayerController::GetActive()
{
	return m_isActive;
}

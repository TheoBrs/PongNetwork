#pragma once
#include <SFML/Window/Event.hpp>
#include "EventCallback.h"

class PLayerController
{
private:
	bool m_isActive;
	bool m_isUpPressed;
	bool m_isDowPressed;
	float m_upAxis;
	void EventKeyPressedCallback(const sf::Event::KeyPressed* event);
	void EventKeyReleasedCallback(const sf::Event::KeyReleased* event);
public:
	void Init();
	Utils::EventCallback<void, float> OnChangeInput;
	void SetActive(bool value);
	bool GetActive();
};


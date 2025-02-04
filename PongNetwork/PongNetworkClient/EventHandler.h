#pragma once
#include <SFML/Window/Event.hpp>

#include "EventCallback.h"

namespace sf
{
    class RenderWindow;
}

class EventHandler
{
public:
    static Utils::EventCallback<void> OnWindowClose;
    static Utils::EventCallback<void, const sf::Event::KeyPressed*> OnKeyPressed;
    static Utils::EventCallback<void, const sf::Event::MouseButtonPressed*> OnMouseButtonPressed;
    static Utils::EventCallback<void, const sf::Event::TextEntered*> OnTextEntered;
    
    static void HandleEvent(sf::RenderWindow* window);
    static sf::Vector2u GetMousePosition(sf::RenderWindow* window);
};

#pragma once
#include <SFML/Window/Event.hpp>

#include "EventCallback.h"

namespace sf
{
    class RenderWindow;
}

class EventHandler
{
private:
    static sf::RenderWindow* s_window;
public:
    static Utils::EventCallback<void> OnWindowClose;
    static Utils::EventCallback<void, const sf::Event::KeyPressed*> OnKeyPressed;
    static Utils::EventCallback<void, const sf::Event::KeyReleased*> OnKeyReleased;
    static Utils::EventCallback<void, const sf::Event::MouseButtonPressed*> OnMouseButtonPressed;
    static Utils::EventCallback<void, const sf::Event::TextEntered*> OnTextEntered;
    
    static void Init(sf::RenderWindow* window);
    static void HandleEvent();
    static sf::Vector2f GetMousePosition();
};

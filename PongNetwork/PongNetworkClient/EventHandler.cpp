#include <SFML/Graphics/RenderWindow.hpp>
#include "EventHandler.h"

Utils::EventCallback<void> EventHandler::OnWindowClose;
Utils::EventCallback<void, const sf::Event::KeyPressed*> EventHandler::OnKeyPressed;
Utils::EventCallback<void, const sf::Event::KeyReleased*> EventHandler::OnKeyReleased;
Utils::EventCallback<void, const sf::Event::MouseButtonPressed*> EventHandler::OnMouseButtonPressed;
Utils::EventCallback<void, const sf::Event::TextEntered*> EventHandler::OnTextEntered;
sf::RenderWindow* EventHandler::s_window = nullptr;

void EventHandler::Init(sf::RenderWindow* window)
{
    s_window = window;
}

void EventHandler::HandleEvent()
{

    while (const std::optional event = s_window->pollEvent())
    {
        
        if (event->is<sf::Event::Closed>())
        {
            OnWindowClose();
            s_window->close();
            
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            OnKeyPressed(keyPressed);
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
            {
                s_window->close();
            }
        }
        else if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>())
        {
            OnMouseButtonPressed(mouseClick);
        }
        else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
        {
            OnTextEntered(textEntered);
        }
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            OnKeyReleased(keyReleased);
        }
    }
    
    
}

sf::Vector2f EventHandler::GetMousePosition()
{
    return sf::Vector2f(sf::Mouse::getPosition(*s_window));
}


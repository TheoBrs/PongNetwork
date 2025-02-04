#include <SFML/Graphics/RenderWindow.hpp>
#include "EventHandler.h"

Utils::EventCallback<void> EventHandler::OnWindowClose;
Utils::EventCallback<void, const sf::Event::KeyPressed*> EventHandler::OnKeyPressed;
Utils::EventCallback<void, const sf::Event::MouseButtonPressed*> EventHandler::OnMouseButtonPressed;
Utils::EventCallback<void, const sf::Event::TextEntered*> EventHandler::OnTextEntered;
void EventHandler::HandleEvent(sf::RenderWindow* window)
{

    while (const std::optional event = window->pollEvent())
    {
        
        if (event->is<sf::Event::Closed>())
        {
            OnWindowClose();
            window->close();
            
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            OnKeyPressed(keyPressed);
            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
            {
                window->close();
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
    }
    
    
}


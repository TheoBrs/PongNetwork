#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "Button.h"
#include "App.h"
void Button::CheckClick(const sf::Event::MouseButtonPressed* event)
{
    if (event->button != sf::Mouse::Button::Left)
    {
        return;
    }
    
    sf::Vector2f mousePosFloat = sf::Vector2f(sf::Mouse::getPosition(*App::Window));
    auto relativeRect = getTransform().transformRect(*m_bounds);

    
    if (relativeRect.contains(mousePosFloat))
    {
        OnClick();
        OnClickEvent();
    }
}



void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform.combine( this->getTransform());
    target.draw(*m_shape, states);
}

void Button::Init(const sf::Vector2f& position, const sf::Vector2f& size)
{
    setPosition(position);
    m_shape = new sf::RectangleShape(size);
    m_bounds = new sf::Rect<float>(sf::Vector2f(0,0), size);
    m_shape->setFillColor(sf::Color::Cyan);
    
    EventHandler::OnMouseButtonPressed += [this](const sf::Event::MouseButtonPressed* event)
    {
        CheckClick(event);
    };

    OnInit();
}

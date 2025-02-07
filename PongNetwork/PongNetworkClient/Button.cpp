#include <SFML/Graphics.hpp>
#include "EventHandler.h"
#include "Button.h"
void Button::CheckClick(const sf::Event::MouseButtonPressed* event)
{
    if (event == nullptr || event->button != sf::Mouse::Button::Left || !m_isActive)
    {
        return;
    }

    if (isMouseHoveringButton())
    {
        OnClick();
        OnClickEvent();
    }
}

bool Button::isMouseHoveringButton()
{
    if (!m_isActive)
    {
        return false;
    }
    sf::Vector2f mousePosFloat = EventHandler::GetMousePosition();
    auto relativeRect = getTransform().transformRect(*m_bounds);
    return  relativeRect.contains(mousePosFloat);
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (!m_isActive)
    {
        return;
    }
    states.transform.combine( this->getTransform());
    target.draw(*m_shape, states);
    target.draw(*m_buttonText, states);
}

void Button::Init(const sf::Vector2f& position, const sf::Vector2f& size, std::string text, sf::Font* font)
{
    setPosition(position);
    m_shape = new sf::RectangleShape(size);
    m_bounds = new sf::Rect<float>(sf::Vector2f(0,0), size);
    m_shape->setFillColor(sf::Color::White);

    m_buttonText = new sf::Text(*font, text);
    auto center = m_buttonText->getGlobalBounds().size / 2.f;
    auto localBounds = center + m_buttonText->getLocalBounds().position;
    auto rounded = sf::Vector2f{ std::round(localBounds.x), std::round(localBounds.y) };
    m_buttonText->setOrigin(rounded);
    m_buttonText->setFillColor(sf::Color::Black);
    m_buttonText->setPosition({m_shape->getSize().x /2.0f, m_shape->getSize().y/2.0f});
    
    EventHandler::OnMouseButtonPressed += [this](const sf::Event::MouseButtonPressed* event)
    {
        CheckClick(event);
    };

    OnInit(font);
}

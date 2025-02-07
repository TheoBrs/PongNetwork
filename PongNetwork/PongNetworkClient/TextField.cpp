#include <iostream>
#include <string>
#include <SFMl/Graphics.hpp>
#include "TextField.h"


#include "EventHandler.h"

void TextField::StartEnterText()
{
    if (m_isEnteringText || !m_isActive)
    {
        return;
    }
    m_isEnteringText = true;
   
    
}

void TextField::EndEnterText()
{
    if (!m_isEnteringText || !m_isActive)
    {
        return;
    }
    m_isEnteringText = false;

    OnValidateText(m_textField->getString());
    m_textField->setString("");
}

void TextField::EnterText(const sf::Event::TextEntered* input)
{
    if (!m_isEnteringText || !m_isActive)
    {
        return;
    }
    std::string str = m_textField->getString();
    if (input->unicode == U'\b' &&  str.length() > 0)
    {
        str = str.substr(0, str.length() - 1);
    }
    else if (input->unicode != U'\b')
    {
        str +=  input->unicode;
    }
    m_textField->setString(str);
    auto center = m_textField->getGlobalBounds().size / 2.f;
    auto localBounds = center + m_textField->getLocalBounds().position;
    auto rounded = sf::Vector2f{ std::round(localBounds.x), std::round(localBounds.y) };
    m_textField->setOrigin(rounded);
}

void TextField::HandleInputKey(const sf::Event::KeyPressed* event)
{
    if (!m_isEnteringText || !m_isActive)
    {
        return;
    }
    if (event->scancode == sf::Keyboard::Scancode::Enter)
    {
        EndEnterText();
    }
}

void TextField::CheckOnClickOutside(const sf::Event::MouseButtonPressed* event)
{
    if (event == nullptr || event->button != sf::Mouse::Button::Left || !m_isEnteringText || isMouseHoveringButton() )
    {
        return;
    }

    m_isEnteringText = false;
}

void TextField::OnClick()
{
    Button::OnClick();
    StartEnterText();
}

void TextField::OnInit(sf::Font* font)
{
    Button::OnInit(font);
    m_callbackOnClickId = EventHandler::OnMouseButtonPressed += [this] (const sf::Event::MouseButtonPressed* event)
    {
        CheckOnClickOutside(event);
    };
    m_callbackTextEnteredId = EventHandler::OnTextEntered += [this](const sf::Event::TextEntered* event)
    {
        EnterText(event);
    };

    m_callbackInputKeyId = EventHandler::OnKeyPressed += [this](const sf::Event::KeyPressed* event)
    {
        HandleInputKey(event);
    };
    m_textField = new sf::Text(*font);
    m_textField->setFillColor(sf::Color::Black);
    m_textField->setPosition({m_shape->getSize().x/2, m_shape->getSize().y/2});

}

void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Button::draw(target, states);
    if (m_textField == nullptr || !m_isActive)
    {
        return;
    }
    states.transform.combine(getTransform());
    target.draw(*m_textField, states);
}

std::string TextField::GetText()
{
    return m_textField->getString();
}

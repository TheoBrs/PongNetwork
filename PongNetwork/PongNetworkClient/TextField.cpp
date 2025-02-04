#include <iostream>
#include <string>
#include <SFMl/Graphics.hpp>
#include "TextField.h"


#include "EventHandler.h"

void TextField::StartEnterText()
{
    if (m_isEnteringText)
    {
        return;
    }
    m_isEnteringText = true;
    m_callbackTextEnteredId = EventHandler::OnTextEntered += [this](const sf::Event::TextEntered* event)
    {
        EnterText(event);
    };

    m_callbackInputKeyId = EventHandler::OnKeyPressed += [this](const sf::Event::KeyPressed* event)
    {
        HandleInputKey(event);
    };
}

void TextField::EndEnterText()
{
    m_isEnteringText = false;

    OnValidateText(m_textField->getString());
    EventHandler::OnTextEntered -= m_callbackTextEnteredId;
    EventHandler::OnKeyPressed -= m_callbackInputKeyId;
    m_textField->setString("");
}

void TextField::EnterText(const sf::Event::TextEntered* input)
{
    std::cout << "Text entered\n";
    std::string str = m_textField->getString();
    if (input->unicode == U'\b' &&  str.length() > 0)
    {
        str = str.substr(0, str.length() - 1);
    }
    else
    {
        str +=  input->unicode;
    }
    m_textField->setString(str);
}

void TextField::HandleInputKey(const sf::Event::KeyPressed* event)
{
    if (event->scancode == sf::Keyboard::Scancode::Enter)
    {
        EndEnterText();
    }
}

void TextField::OnClick()
{
    Button::OnClick();
    StartEnterText();
}

void TextField::OnInit()
{
    Button::OnInit();
    m_font = new sf::Font();
    if (!m_font->openFromFile("Fonts/arial/arial.ttf"))
    {
        std::cout << "Failed to load font\n";
        return;
    }
    m_textField = new sf::Text(*m_font);
   
}

void TextField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    Button::draw(target, states);
    if (m_textField == nullptr)
    {
        return;
    }
    states.transform.combine(getTransform());
    target.draw(*m_textField, states);
}

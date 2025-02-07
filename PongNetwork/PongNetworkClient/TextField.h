#pragma once
#include <SFML/Window/Event.hpp>
#include "Button.h"
#include "EventCallback.h"



namespace sf
{
    class Text;
    class Font;

}
class TextField : public Button
{
private:
    bool m_isEnteringText;
    size_t m_callbackTextEnteredId;
    size_t m_callbackInputKeyId;
    size_t m_callbackOnClickId;
    sf::Text* m_textField;
    int m_maxCharacter = 100;

    void StartEnterText();
    void EndEnterText();
    void EnterText(const sf::Event::TextEntered* input);
    void HandleInputKey(const sf::Event::KeyPressed* event);
    void CheckOnClickOutside(const sf::Event::MouseButtonPressed* event);
    
protected:
    void OnClick() override;
    void OnInit(sf::Font* font) override;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    Utils::EventCallback<void, std::string> OnValidateText;
    std::string GetText();
    void SetMaxCharacter(int value);
};

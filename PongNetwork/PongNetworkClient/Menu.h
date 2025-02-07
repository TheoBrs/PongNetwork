#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include "EventCallback.h"

class TextField;
class Button;

namespace sf 
{
	class Text;
	class Font;
}


class Menu: public sf::Drawable
{
private:
	TextField* m_nameField;
	TextField* m_addrField;
	Button* m_validationButton;
	sf::Text* m_errorText;
	sf::Text* m_adressText;
	sf::Text* m_nameText;

	void OnValidate();
protected:
	bool m_isActive;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	void Init(sf::Font* font);

	void SetErrorText(const std::string& text);
	Utils::EventCallback<void, std::string, int, std::string> OnValidateEvent;

	bool GetIsActive();
	void SetActive(bool value);
	// Hérité via Drawable
};


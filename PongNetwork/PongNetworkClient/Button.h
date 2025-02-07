#pragma once
#include <functional>
#include <vector>
#include <SFML/Window/Event.hpp>
#include "AEntity.h"
#include "EventCallback.h"

namespace sf
{
	class RectangleShape;
	class Text;
	class Font;
}
class Button : public AEntity
{
private:
	sf::Rect<float>* m_bounds = nullptr;
	sf::Text* m_buttonText;
	void CheckClick(const sf::Event::MouseButtonPressed* event);
protected:
	bool isMouseHoveringButton();
	sf::RectangleShape* m_shape = nullptr;
	virtual void OnClick() {};
	virtual void OnInit(sf::Font* font) {};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Utils::EventCallback<void> OnClickEvent;
	void Init(const sf::Vector2f& position, const sf::Vector2f& size, std::string text, sf::Font* font);
};


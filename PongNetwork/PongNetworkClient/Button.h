#pragma once
#include <functional>
#include <vector>
#include <SFML/Window/Event.hpp>
#include "AEntity.h"
#include "EventCallback.h"

namespace sf
{
	class RectangleShape;
}
class Button : public AEntity
{
private:
	sf::Rect<float>* m_bounds = nullptr;
	void CheckClick(const sf::Event::MouseButtonPressed* event);
protected:
	sf::RectangleShape* m_shape = nullptr;
	virtual void OnClick() {};
	virtual void OnInit() {};
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
public:
	Utils::EventCallback<void> OnClickEvent;
	void Init(const sf::Vector2f& position, const sf::Vector2f& size);
};


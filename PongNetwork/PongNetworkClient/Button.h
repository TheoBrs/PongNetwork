#pragma once
#include <functional>
#include <vector>
#include "AEntity.h"

class Button : public AEntity
{
private:
	sf::RectangleShape* m_shape;
	


protected:
	// H�rit� via AEntity
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

};


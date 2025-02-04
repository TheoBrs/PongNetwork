#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace sf
{
	class RenderTarget;
	struct RenderStates;
}

class AEntity : public sf::Transformable, public sf::Drawable
{

};


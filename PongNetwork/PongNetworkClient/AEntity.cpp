#include "AEntity.h"
#include <SFML/Graphics.hpp>

void AEntity::SetActive(bool value)
{
	m_isActive = value;
}

bool AEntity::GetIsActive()
{
	return m_isActive;
}

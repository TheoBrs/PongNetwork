#include <SFML/Graphics.hpp>
#include <sstream>

#include "Menu.h"
#include "TextField.h"
#include "Button.h"


void Menu::OnValidate()
{
	if (!m_isActive)
	{
		return;
	}
	std::string name = m_nameField->GetText();
	std::string addr = m_addrField->GetText();
	std::stringstream ss(addr);
	std::string ip;
	int port;
	std::getline(ss, ip, ':');
	ss >> port;

	OnValidateEvent(ip, port, name);
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!m_isActive)
	{
		return;
	}
	target.draw(*m_nameField, states);
	target.draw(*m_addrField, states);
	target.draw(*m_validationButton, states);
	target.draw(*m_errorText, states);

}

void Menu::Init(sf::Font* font)
{
	m_nameField = new TextField();
	m_nameField->Init({ 625,145 },{ 350,75 }, "", font);

	m_addrField = new TextField();
	m_addrField->Init({ 625,245 }, { 350,75 }, "", font);

	m_validationButton = new Button();
	m_validationButton->Init({ 625,345 }, { 350,75 }, "Validate", font);

	m_errorText = new sf::Text(*font);
	SetErrorText("Error Text");
	m_errorText->setPosition({ 800.f, 500.f });

	m_validationButton->OnClickEvent += [this]() {OnValidate(); };
}

void Menu::SetErrorText(const std::string& text)
{
	auto center = m_errorText->getGlobalBounds().size / 2.f;
	auto localBounds = center + m_errorText->getLocalBounds().position;
	auto rounded = sf::Vector2f{ std::round(localBounds.x), std::round(localBounds.y) };
	m_errorText->setOrigin(rounded);

}

bool Menu::GetIsActive()
{
	return m_isActive;
}

void Menu::SetActive(bool value)
{
	m_isActive = value;
	m_nameField->SetActive(value);
	m_addrField->SetActive(value);
	m_validationButton->SetActive(value);
}



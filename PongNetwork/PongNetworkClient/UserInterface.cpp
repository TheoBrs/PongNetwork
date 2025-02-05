#include <SFML/Graphics/RenderTarget.hpp>
#include "UserInterface.h"
#include "App.h"

void UserInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto player: m_players)
    {
        target.draw(*player.second->Name);
        target.draw(*player.second->Score);
        target.draw(*player.second->ConnectionState);
    }
}

void UserInterface::Init()
{
}

void UserInterface::AddPlayer(int id, std::string name, bool isConnected)
{
    PlayerInfos* newPlayer = new PlayerInfos() ;
    newPlayer->Name = new sf::Text(*App::MainFont);
    newPlayer->Name->setString(name);
    newPlayer->Score = new sf::Text(*App::MainFont);
    newPlayer->Score->setString("0");
    newPlayer->ConnectionState = new sf::CircleShape(CONNECTION_STATE_SIZE);
    sf::Color color = isConnected ? sf::Color::Green : sf::Color::Red;
    newPlayer->ConnectionState->setFillColor(color);
    if (id == 0)
    {
        newPlayer->Name->setPosition(PLAYER_NAME_POSITION_0);
        newPlayer->ConnectionState->setPosition(PLAYER_CONNECTION_STATE_POSITION_0);
        newPlayer->Score->setPosition(PLAYER_SCORE_POSITION_0);
    }
    else if (id == 1)
    {
        newPlayer->Name->setPosition(PLAYER_NAME_POSITION_1);
        newPlayer->ConnectionState->setPosition(PLAYER_CONNECTION_STATE_POSITION_1);
        newPlayer->Score->setPosition(PLAYER_SCORE_POSITION_1);
    }
    m_players[id] = newPlayer;
}

void UserInterface::SetPlayerConnection(int id, bool isConnected)
{
    sf::Color color = isConnected ? sf::Color::Green : sf::Color::Red;
    m_players[id]->ConnectionState->setFillColor(color);
}

void UserInterface::SetPlayerName(int id, std::string name)
{
    m_players[id]->Name->setString(name);
}

void UserInterface::SetPlayerScore(int id, int score)
{
    m_players[id]->Score->setString(std::to_string(score));
}

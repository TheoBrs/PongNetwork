#pragma once
#include <unordered_map>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>


inline constexpr sf::Vector2f PLAYER_NAME_POSITION_0 = {100,10};
inline constexpr sf::Vector2f PLAYER_CONNECTION_STATE_POSITION_0 = {250,20};
inline constexpr sf::Vector2f PLAYER_SCORE_POSITION_0 = {700,10};

inline constexpr sf::Vector2f PLAYER_NAME_POSITION_1 = {1400,10};
inline constexpr sf::Vector2f PLAYER_CONNECTION_STATE_POSITION_1 = {1350,20};
inline constexpr sf::Vector2f PLAYER_SCORE_POSITION_1 = {900,10};

inline constexpr float CONNECTION_STATE_SIZE = 10.f;

class UserInterface : public sf::Drawable
{
    struct PlayerInfos
    {
        sf::CircleShape* ConnectionState;
        sf::Text* Name;
        sf::Text* Score;
    };
    
private:
    std::unordered_map<int, PlayerInfos*> m_players;
    sf::Font* m_font;
protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    void Init(sf::Font* font);
    void AddPlayer(int id, std::string name, bool isConnected);
    void SetPlayerConnection(int id, bool isConnected);
    void SetPlayerName(int id, std::string name);
    void SetPlayerScore(int id, int score);
};
// code qui fait des _Struct_size_bytes_(
//     appeler code qui fait des trucs
//     feur
//     )
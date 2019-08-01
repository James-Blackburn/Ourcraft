#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Player : public sf::Sprite
{
public:
    int x, y;
    int moveY = 0;
    int moveX = 0;
};

#endif // PLAYER_HPP_INCLUDED

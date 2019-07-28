#ifndef GAMESTATE_HPP_INCLUDED
#define GAMESTATE_HPP_INCLUDED

#include "Game.hpp"

class GameState
{
protected:
    Game* game;
public:
    virtual void draw(){}
    virtual void update(){}
};

#endif // GAMESTATE_HPP_INCLUDED

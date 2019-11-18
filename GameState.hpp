#ifndef GAMESTATE_HPP_INCLUDED
#define GAMESTATE_HPP_INCLUDED

class Game;

enum GameStates
{
    MENU_STATE, PLAYING_STATE
};

class GameState
{
protected:
    Game* game;
public:
    virtual void draw() = 0;
    virtual void update() = 0;
};

#endif // GAMESTATE_HPP_INCLUDED

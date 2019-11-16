#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "GameState.hpp"
#include <memory>

class GameState;

class Game
{
private:
    std::unique_ptr<GameState> currentState;
    int currentStateID;

public:
    sf::RenderWindow window;
    sf::Font gameFont;
    sf::Font miscFont;

    int windowX;
    int windowY;

    Game(int desktopWidth, int desktopHeight);
    void gameLoop();
    void loadResources();
    void changeState(int newStateID);
    void handleStates(int prevStateID);
};

#endif // GAME_HPP_INCLUDED

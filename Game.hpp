#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include <stack>

class GameState;

class Game
{
public:
    sf::RenderWindow window;
    std::stack<GameState*> stateStack;
    GameState* currentState;
    sf::Font gameFont;
    sf::Font miscFont;

    Game();
    void gameLoop();
    void loadResources();
    void changeState(GameState*);
};

#endif // GAME_HPP_INCLUDED

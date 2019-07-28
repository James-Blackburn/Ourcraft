#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "MenuState.hpp"
#include "Constants.hpp"

Game::Game() : window(sf::VideoMode(WINDOW_X, WINDOW_Y), "Ourcraft 1.0")
{
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    loadResources();
    currentState = new MenuState(this);
}


void Game::loadResources()
{
    gameFont.loadFromFile("fnt/Minecrafter.Reg.ttf");
    miscFont.loadFromFile("fnt/ArialCE.ttf");
}

void Game::changeState(GameState* newState)
{
    stateStack.push(newState);
}

void Game::gameLoop()
{
    currentState->update();
    currentState->draw();
    if (!stateStack.empty())
    {
        delete currentState;
        currentState = stateStack.top();
        stateStack.pop();
    }
}

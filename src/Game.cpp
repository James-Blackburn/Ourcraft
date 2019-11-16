#include <SFML/Graphics.hpp>
#include "Game.hpp"
#include "MenuState.hpp"
#include "PlayingState.hpp"
#include "Constants.hpp"

Game::Game(int desktopWidth, int desktopHeight) :
    windowX(desktopWidth), windowY(desktopHeight),
    window(sf::VideoMode(desktopWidth, desktopHeight), "Ourcraft 1.0", sf::Style::Fullscreen),
    currentStateID(GameStates::MENU_STATE)
{
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    loadResources();
    currentState = std::make_unique<MenuState>(this);
}


void Game::loadResources()
{
    gameFont.loadFromFile("fnt/Minecrafter.Reg.ttf");
    miscFont.loadFromFile("fnt/ArialCE.ttf");
}

void Game::changeState(int newStateID)
{
    currentStateID = newStateID;
}

void Game::gameLoop()
{
    int prevStateID = currentStateID;
    currentState->update();
    currentState->draw();
    handleStates(prevStateID);
}

void Game::handleStates(int prevStateID)
{
    if (prevStateID != currentStateID)
    {
        switch(currentStateID)
        {
            case GameStates::MENU_STATE:
                currentState = std::make_unique<MenuState>(this);
                break;
            case GameStates::PLAYING_STATE:
                currentState = std::make_unique<PlayingState>(this);
                break;
        }
    }
}

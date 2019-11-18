#include <SFML/Graphics.hpp>
#include "MenuState.hpp"
#include "PlayingState.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Game.hpp"

MenuState::MenuState(Game* game_)
{
    game = game_;

    // Make Buttons
    playButton = std::make_unique<Button>(game->windowX / 2, 300, "img/button.png");
    exitButton = std::make_unique<Button>(game->windowX / 2, 400, "img/button.png");

    // Make Text
    titleText = std::make_unique<Text>(game->gameFont, "OURCRAFT", 72, sf::Color::White, sf::Vector2f(game->windowX / 2, 100));
    playText = std::make_unique<Text>(game->gameFont, "PLAY", 48, sf::Color::Green, sf::Vector2f(game->windowX / 2, 300));
    exitText = std::make_unique<Text>(game->gameFont, "EXIT", 48, sf::Color::Red, sf::Vector2f(game->windowX / 2, 400));
}

MenuState::~MenuState() = default;

void MenuState::update()
{
    sf::Event event;
    while (game->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            game->window.close();

        // Check mouse events
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mpos = sf::Vector2i(sf::Mouse::getPosition(game->window));
                if (exitButton->clicked(mpos.x, mpos.y))
                    game->window.close();
                else if (playButton->clicked(mpos.x, mpos.y))
                    game->changeState(GameStates::PLAYING_STATE);
            }
        }
    }
}

void MenuState::draw()
{
    game->window.clear();
    game->window.draw(*playButton);
    game->window.draw(*exitButton);
    titleText->draw(game->window);
    playText->draw(game->window);
    exitText->draw(game->window);
    game->window.display();
}

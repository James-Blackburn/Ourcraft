#include <SFML/Graphics.hpp>
#include "MenuState.hpp"
#include "PlayingState.hpp"
#include "Button.hpp"
#include "Text.hpp"

MenuState::MenuState(Game* game_)
{
    game = game_;
    menuBackgroundTexture.loadFromFile("img/menu.png");
    menuBackgroundSprite.setTexture(menuBackgroundTexture);
    playButton = new Button(sf::Vector2f(340,225), sf::Vector2f(335,75), sf::Color(105,105,105));
    exitButton = new Button(sf::Vector2f(340,395), sf::Vector2f(335,75), sf::Color(105,105,105));
    playText = new Text(game->gameFont, "PLAY", 48, sf::Color::Green, sf::Vector2f(440, 230));
    exitText = new Text(game->gameFont, "EXIT", 48, sf::Color::Red, sf::Vector2f(440, 400));
}

MenuState::~MenuState()
{
    delete playButton;
    delete exitButton;
    delete playText;
    delete exitText;
}

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
                sf::Vector2f mpos = sf::Vector2f(sf::Mouse::getPosition(game->window));
                if (exitButton->clicked(mpos)) game->window.close();
                if (playButton->clicked(mpos)) game->changeState(new PlayingState(game));
            }
        }
    }
}

void MenuState::draw()
{
    game->window.clear();
    game->window.draw(menuBackgroundSprite);
    playButton->draw(game->window);
    exitButton->draw(game->window);
    playText->draw(game->window);
    exitText->draw(game->window);
    game->window.display();
}

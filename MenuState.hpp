#ifndef MENUSTATE_HPP_INCLUDED
#define MENUSTATE_HPP_INCLUDED

#include <SFML/Audio.hpp>
#include "GameState.hpp"

class Button;
class Text;

class MenuState : public GameState
{
private:
    sf::Texture menuBackgroundTexture;
    sf::Sprite menuBackgroundSprite;
    Button* playButton;
    Button* exitButton;
    Text* playText;
    Text* exitText;
public:
    MenuState(Game*);
    ~MenuState();
    void update();
    void draw();
};

#endif // MENUSTATE_HPP_INCLUDED

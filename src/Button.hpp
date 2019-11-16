#ifndef BUTTON_HPP_INCLUDED
#define BUTTON_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include "Sprite.hpp"

class Button : public Sprite
{
public:
    int x, y;
    sf::IntRect rect;

    Button(int x_, int y_, const char* imgPath);
    bool clicked(int mouseX, int mouseY);
};

#endif // BUTTON_HPP_INCLUDED

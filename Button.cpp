#include "Button.hpp"
#include <iostream>

Button::Button(int x_, int y_, const char* imgPath)
    : x(x_), y(y_)
{
    this->loadFromFile(imgPath);
    sf::Vector2u textureSize = spriteTexture.getSize();
    x = x - (textureSize.x / 2);    // <- Center button
    this->setPosition(this->x, this->y);
    this->rect = sf::IntRect(this->x, this->y, textureSize.x, textureSize.y);
}

bool Button::clicked(int mouseX, int mouseY)
{
    if (this->rect.contains(mouseX, mouseY))
        return true;
    return false;
}

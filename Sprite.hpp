#ifndef SPRITE_HPP_INCLUDED
#define SPRITE_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Sprite : public sf::Sprite
{
protected:
    sf::Texture spriteTexture;
public:
    Sprite(){}
    Sprite(const char* imgPath);
    virtual void loadFromFile(const char* imgPath);
};

#endif // SPRITE_HPP_INCLUDED

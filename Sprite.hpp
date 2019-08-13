#ifndef SPRITE_HPP_INCLUDED
#define SPRITE_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Sprite : public sf::Sprite
{
private:
    sf::Texture spriteTexture;
public:
    Sprite(){}
    Sprite(const char* imgPath);
    void loadFromFile(const char* imgPath);
};

#endif // SPRITE_HPP_INCLUDED

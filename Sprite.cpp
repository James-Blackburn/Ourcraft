#include "Sprite.hpp"

Sprite::Sprite(const char* imgPath)
{
    loadFromFile(imgPath);
}

void Sprite::loadFromFile(const char* imgPath)
{
    spriteTexture.loadFromFile(imgPath);
    setTexture(spriteTexture);
}

#ifndef RESOURCEMANAGER_HPP_INCLUDED
#define RESOURCEMANAGER_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class ResourceManager
{
public:
    void loadSprite(sf::Texture, sf::Sprite)
public:
    sf::Texture menuTexture;
    sf::Sprite menuSprite;

    sf::Texture dirtTexture;
    sf::Sprite dirtSprite;

    sf::Texture grassTexture;
    sf::Sprite grassSprite;

    sf::Texture stoneTexture;
    sf::Sprite stoneSprite;

    sf::Texture waterTexture;
    sf::Sprite waterSprite;

    sf::Texture sandTexture;
    sf::Sprite sandSprite;

    sf::Texture emptyTexture;
    sf::Sprite emptySprite;
};

#endif // RESOURCEMANAGER_HPP_INCLUDED

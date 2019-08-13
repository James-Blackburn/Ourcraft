#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <SFML/Graphics/Rect.hpp>

enum TileType
{
    GRASS, DIRT, STONE, SAND, WATER, TREEWOOD, LEAF
};

struct Tile
{
    int x, y;
    int type;
    int priority;
    int depth;

    sf::IntRect rect;

    Tile(int x_, int y_, int depth_, int type_, int priority_);
    int getBrightness();
};

#endif // TILE_HPP_INCLUDED

#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Player;

enum TileType
{
    DIRT, GRASS, SAND, STONE, TREEWOOD, LEAF, WATER
};

struct Tile
{
    int x, y;
    int type;
    int priority;
    int depth;

    sf::IntRect rect;

    Tile(int x_, int y_, int depth_, int type_, int priority_);
    int getBrightness(Player* player, int xPos, int yPos);
};

#endif // TILE_HPP_INCLUDED

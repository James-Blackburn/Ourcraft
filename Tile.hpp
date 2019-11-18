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
	unsigned char* quadsBrightness = nullptr;

    sf::IntRect rect;

    Tile(int x_, int y_, int depth_, int type_, int priority_);
	int getQuadBrightness(Player* player, int quadType);
};

#endif // TILE_HPP_INCLUDED

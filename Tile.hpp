#ifndef TILE_HPP_INCLUDED
#define TILE_HPP_INCLUDED

struct Tile
{
    int x, y;
    int type;
    int health;
    int depth;

    Tile(int x_, int y_, int depth_, int type_, int health_);
};

#endif // TILE_HPP_INCLUDED

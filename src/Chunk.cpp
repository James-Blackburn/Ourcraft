#include "Chunk.hpp"
#include "Tile.hpp"

Chunk::Chunk(int rectLeft, int rectTop, int rectWidth, int rectHeight)
    : rect(rectLeft,rectTop,rectWidth,rectHeight)
{
}

void Chunk::removeTile(int tileIndex)
{
    tiles.erase(tiles.begin() + tileIndex);
}


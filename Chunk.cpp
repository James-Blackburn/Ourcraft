#include "Chunk.hpp"
#include "Tile.hpp"

Chunk::Chunk(int rectLeft, int rectTop, int rectWidth, int rectHeight)
            : rect(rectLeft,rectTop,rectWidth,rectHeight)
{

}

Chunk::~Chunk()
{
    for (Tile* tile : tiles)
    {
        delete tile;
    }
}

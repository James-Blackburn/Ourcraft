#ifndef CHUNK_HPP_INCLUDED
#define CHUNK_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

struct Tile;

struct Chunk
{
    std::vector<Tile*> tiles;
    sf::IntRect rect;

    Chunk(int rectLeft, int rectTop, int rectWidth, int rectHeight);
    ~Chunk();
};

#endif // CHUNK_HPP_INCLUDED

#ifndef CHUNK_HPP_INCLUDED
#define CHUNK_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

struct Tile;

struct Chunk
{
    std::vector<std::shared_ptr<Tile>> tiles;
    sf::IntRect rect;

    Chunk(int rectLeft, int rectTop, int rectWidth, int rectHeight);
    void removeTile(int tileIndex);
};

#endif // CHUNK_HPP_INCLUDED

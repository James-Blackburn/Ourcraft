#ifndef MAPGENERATOR_HPP_INCLUDED
#define MAPGENERATOR_HPP_INCLUDED

#include <vector>

class Player;
class Chunk;
class Tile;

class MapGenerator
{
private:
    void spawnPlayer(std::vector<int>& blockHeights, Player* player, int waterLevel);
    void generateTree(std::vector<Tile*>& tiles, int x, int y);
    void allocateChunks(std::vector<Chunk*>& chunks, std::vector<Tile*>& tiles);
public:
    void generateMap(std::vector<Chunk*>& chunks, Player* player, bool& generated);
};

#endif // MAPGENERATOR_HPP_INCLUDED

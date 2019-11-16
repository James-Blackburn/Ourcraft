#ifndef MAPGENERATOR_HPP_INCLUDED
#define MAPGENERATOR_HPP_INCLUDED

#include <memory>
#include <vector>
#include <string>

class Player;
class Chunk;
class Tile;

class MapGenerator
{
private:
    void spawnPlayer(std::vector<int>& blockHeights, std::unique_ptr<Player>& player, int waterLevel);
    void generateTree(std::vector<std::shared_ptr<Tile>>& tiles, int x, int y);
    void allocateChunks(std::vector<std::unique_ptr<Chunk>>& chunks, std::vector<std::shared_ptr<Tile>>& tiles);
public:
    void generateMap(std::vector<std::unique_ptr<Chunk>>& chunks, std::unique_ptr<Player>& player, bool& generated, std::string& currentText);
};

#endif // MAPGENERATOR_HPP_INCLUDED

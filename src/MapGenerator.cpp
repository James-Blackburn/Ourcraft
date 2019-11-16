#include "MapGenerator.hpp"
#include "NoiseGenerator.hpp"
#include "Constants.hpp"
#include "Utilities.hpp"
#include "Tile.hpp"
#include "Chunk.hpp"
#include "Player.hpp"

#include <cstdlib>
#include <ctime>
#include <vector>
#include <mutex>

void MapGenerator::generateMap(std::vector<std::unique_ptr<Chunk>>& chunks, std::unique_ptr<Player>& player, bool& generated, std::string& currentText)
{
    static std::mutex mtx;
    NoiseGenerator generator;
    generator.generatePerlinNoise(8);
    long long int totalHeight = 0;

    currentText = "Loading Block Heights";
    // Determine min, average and max block heights
    std::vector<int> blockHeights;
    blockHeights.reserve(MAP_BLOCK_WIDTH);
    for (int x=0; x<MAP_X; x+=BLOCK_SIZE)
    {
        int height =  (MAP_Y * 0.75) - (generator.perlinNoise[x/BLOCK_SIZE] * (MAP_Y / 2));
        int remainder = height % BLOCK_SIZE;
        height = (height + BLOCK_SIZE - remainder);
        blockHeights.push_back(height);
        totalHeight += height;
    }

    int averageHeight = totalHeight / MAP_BLOCK_WIDTH;
    int waterLevel = averageHeight + (BLOCK_SIZE*2);
    bool treeLastBlock = false;

    currentText = "Generating Tiles";
    int blockHeightIndex = 0;
    std::vector<std::shared_ptr<Tile>> tiles;
    for (int x=0; x<MAP_X; x+=BLOCK_SIZE)
    {
        int height = blockHeights[blockHeightIndex];

        bool nextToWater = false;
        if (blockHeights[blockHeightIndex+1] > waterLevel + BLOCK_SIZE
            || blockHeights[blockHeightIndex-1] > waterLevel + BLOCK_SIZE)
            nextToWater = true;

        // Set block depths
        int blockDepth = 0;
        int stoneBlockDepth = randomInt(6,7);

        // Determine block types
        for (int y=height; y<MAP_Y; y+=BLOCK_SIZE)
        {
            int tileType = (nextToWater) ? TileType::SAND : TileType::GRASS;
            if (blockDepth > 0 && blockDepth < stoneBlockDepth)
                tileType = (nextToWater) ? TileType::SAND : TileType::DIRT;
            else if (blockDepth >= stoneBlockDepth)
                tileType = TileType::STONE;
            std::shared_ptr<Tile> newTile = std::make_shared<Tile>(x, y, blockDepth, tileType, 0);
            tiles.push_back(newTile);
            blockDepth++;
        }

        // Place water
        if (height > waterLevel)
        {
            for (int y=height-BLOCK_SIZE; y>waterLevel; y-=BLOCK_SIZE)
            {
                std::shared_ptr<Tile> newTile = std::make_shared<Tile>(x, y, 0, TileType::WATER, 0);
                tiles.push_back(newTile);
            }
        }
        // Place Trees
        else if (height < waterLevel)
        {
            bool placeTree = (randomInt(1,20) <= (10*TREE_CHANCE));
            if (placeTree && !treeLastBlock)
            {
                generateTree(tiles, x, height);
                treeLastBlock = true;
            } else
                treeLastBlock = false;
        }
        blockHeightIndex++;
    }

    currentText = "Allocating Chunks";
    allocateChunks(chunks, tiles);
    currentText = "Spawning Player";
    spawnPlayer(blockHeights, player, waterLevel);
    generated = true;
}

void MapGenerator::allocateChunks(std::vector<std::unique_ptr<Chunk>>& chunks, std::vector<std::shared_ptr<Tile>>& tiles)
{
    int y = 0;
    for (int x=0; x<MAP_X; x+=CHUNK_SIZE*BLOCK_SIZE)
    {
        std::unique_ptr<Chunk> newChunk(std::make_unique<Chunk>(x, y, CHUNK_SIZE*BLOCK_SIZE, MAP_Y));
        for (int i=0; i<tiles.size(); i++)
        {
            if (newChunk->rect.contains(tiles[i]->x + BLOCK_SIZE/2, tiles[i]->y + BLOCK_SIZE/2))
            {
                newChunk->tiles.push_back(tiles[i]);
            }
        }

        // Rearrange tiles based on rendering priority
        std::sort(newChunk->tiles.begin(), newChunk->tiles.end(),
            [](std::shared_ptr<Tile>& tile1, std::shared_ptr<Tile>& tile2) -> bool
            { return tile1->priority < tile2->priority; });

        // Remove leaf overlap
        for (int leafIndex = 0; leafIndex < newChunk->tiles.size(); leafIndex++)
        {
            if (newChunk->tiles[leafIndex]->type != TileType::LEAF)
                continue;
            for (int tileIndex = 0; tileIndex < newChunk->tiles.size(); tileIndex++)
            {
                if (newChunk->tiles[tileIndex]->type == TileType::TREEWOOD || tileIndex == leafIndex)
                    continue;
                if (newChunk->tiles[leafIndex]->rect.intersects(newChunk->tiles[tileIndex]->rect))
                {
                    newChunk->tiles.erase(newChunk->tiles.begin() + leafIndex);
                    leafIndex--;
                }
            }
        }
        chunks.push_back(std::move(newChunk));
    }
}

void MapGenerator::generateTree(std::vector<std::shared_ptr<Tile>>& tiles, int x, int treeBaseY)
{
    int treeHeight = treeBaseY - (randomInt(10, 20) * BLOCK_SIZE);
    float treeBlockDepth = 0.0f;
    for (int y = treeHeight; y < treeBaseY; y += BLOCK_SIZE)
    {
        // Place Leaves
        int roundedTreeBlockDepth = static_cast<int>(treeBlockDepth);
        if (roundedTreeBlockDepth < 7)
        {
            int offset = roundedTreeBlockDepth * BLOCK_SIZE;
            for (int leafX = x - offset; leafX <= x + offset; leafX += BLOCK_SIZE)
            {
                std::shared_ptr<Tile> newTile = std::make_shared<Tile>(leafX, y - BLOCK_SIZE, 0, TileType::LEAF, 1);
                tiles.push_back(newTile);
            }
        }
        // Place Wood
        std::shared_ptr<Tile> newTile = std::make_shared<Tile>(x, y, 0, TileType::TREEWOOD, 0);
        tiles.push_back(newTile);
        treeBlockDepth += 0.5;
    }
}

void MapGenerator::spawnPlayer(std::vector<int>& blockHeights, std::unique_ptr<Player>& player, int waterLevel)
{
    int midpoint = static_cast<int>((blockHeights.size() - 1) / 2);
    for (int i = 0; i >= 0; i++)
    {
        if (blockHeights[midpoint - i] <= waterLevel)
        {
            player->x = (MAP_X/2) - (BLOCK_SIZE * i);
            player->y = blockHeights[midpoint - i] - PLAYER_HEIGHT;
            break;
        }
        else if (blockHeights[midpoint + i] <= waterLevel)
        {
            player->x = (MAP_X/2) + (BLOCK_SIZE * i);
            player->y = blockHeights[midpoint + i] - PLAYER_HEIGHT;
            break;
        }
    }
}

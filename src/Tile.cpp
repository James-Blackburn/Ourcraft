#include "Tile.hpp"
#include "Constants.hpp"
#include "Player.hpp"

Tile::Tile(int x_, int y_, int depth_, int type_, int priority_) :
    x(x_), y(y_), depth(depth_), type(type_), priority(priority_)
    , rect(x_, y_, BLOCK_SIZE, BLOCK_SIZE)
{
}

int Tile::getBrightness(Player* player, int xPos, int yPos)
{
    int xDistance = std::abs(player->x - xPos);
    int yDistance = std::abs(player->y - yPos);
    int distance =  std::sqrt((xDistance * xDistance) + (yDistance * yDistance));
    int brightness = MAX_TILE_BRIGHTNESS - distance / 2;

    if (brightness < 0)
        brightness = 0;

    return brightness;
}

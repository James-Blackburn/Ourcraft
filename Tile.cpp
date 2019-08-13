#include "Tile.hpp"
#include "Constants.hpp"

Tile::Tile(int x_, int y_, int depth_, int type_, int priority_) :
    x(x_), y(y_), depth(depth_), type(type_), priority(priority_)
    , rect(x_, y_, BLOCK_SIZE, BLOCK_SIZE){}

int Tile::getBrightness()
{
    int brightness = MAX_TILE_BRIGHTNESS;
    if (depth > SHADOW_DEPTH)
        brightness = MAX_TILE_BRIGHTNESS - ((depth-SHADOW_DEPTH)*SHADOW_SCALING_FACTOR);
    else
        return brightness;

    if (brightness < 0)
        brightness = 0;
    return brightness;
}

#include "Tile.hpp"
#include "Constants.hpp"
#include "Player.hpp"

Tile::Tile(int x_, int y_, int depth_, int type_, int priority_) :
    x(x_), y(y_), depth(depth_), type(type_), priority(priority_)
    , rect(x_, y_, BLOCK_SIZE, BLOCK_SIZE)
{
	quadsBrightness = new unsigned char[4];
	for (int i = 0; i < 4; i++)
	{
		quadsBrightness[i] = 0;
	}
}

int Tile::getQuadBrightness(Player* player, int quadType)
{
	int xDistance;
	int yDistance;
	
	switch (quadType)
	{
	case 0:
		xDistance = std::abs((player->x + PLAYER_WIDTH / 2) - x);
		yDistance = std::abs((player->y + PLAYER_HEIGHT / 2) - y);
		break;
	case 1:
		xDistance = std::abs((player->x + PLAYER_WIDTH / 2) - (x + BLOCK_SIZE));
		yDistance = std::abs((player->y + PLAYER_HEIGHT / 2) - y);
		break;
	case 2:
		xDistance = std::abs((player->x + PLAYER_WIDTH / 2) - (x + BLOCK_SIZE));
		yDistance = std::abs((player->y + PLAYER_HEIGHT / 2) - (y + BLOCK_SIZE));
		break;
	case 3:
		xDistance = std::abs((player->x + PLAYER_WIDTH / 2) - x);
		yDistance = std::abs((player->y + PLAYER_HEIGHT / 2) - (y + BLOCK_SIZE));
		break;
	}
	
	int distance = std::sqrt((xDistance * xDistance) + (yDistance * yDistance));

	int newQuadBrightness = MAX_TILE_BRIGHTNESS - distance;
	if (newQuadBrightness < quadsBrightness[quadType])
		newQuadBrightness = quadsBrightness[quadType];
	else if (newQuadBrightness > MAX_TILE_BRIGHTNESS)
		newQuadBrightness = MAX_TILE_BRIGHTNESS;
	
	quadsBrightness[quadType] = newQuadBrightness;
	return newQuadBrightness;
}

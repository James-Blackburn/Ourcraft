#ifndef PLAYINGSTATE_HPP_INCLUDED
#define PLAYINGSTATE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "GameState.hpp"
#include "Chunk.hpp"
#include "Sprite.hpp"

#include "Constants.hpp"

class Text;
class Player;

class PlayingState : public GameState
{
private:
    std::vector<Chunk*> chunks;
    Text* fpsText;
    sf::Clock clock;

    int camera_x = MAP_X/2;
    int camera_y = 0;
    int moveCameraX = 0;
    int moveCameraY = 0;
    bool generated = false;

    Player* player;

    Sprite dirtSprite;
    Sprite grassSprite;
    Sprite stoneSprite;
    Sprite waterSprite;
    Sprite sandSprite;
    Sprite treeWoodSprite;
    Sprite leafSprite;
    Sprite emptySprite;

public:
    PlayingState(Game*);
    ~PlayingState();
    void loadResources();
    void loadMap();
    void update();
    void draw();
};

#endif // PLAYINGSTATE_HPP_INCLUDED

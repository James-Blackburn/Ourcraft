#ifndef PLAYINGSTATE_HPP_INCLUDED
#define PLAYINGSTATE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "GameState.hpp"
#include "Chunk.hpp"

#include "Constants.hpp"

class Text;

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

    sf::Texture dirtTexture;
    sf::Sprite dirtSprite;

    sf::Texture grassTexture;
    sf::Sprite grassSprite;

    sf::Texture stoneTexture;
    sf::Sprite stoneSprite;
public:
    PlayingState(Game*);
    ~PlayingState();
    void loadResources();
    void loadMap();
    void generateMap();
    void update();
    void draw();
};

#endif // PLAYINGSTATE_HPP_INCLUDED

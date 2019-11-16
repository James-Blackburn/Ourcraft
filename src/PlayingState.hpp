#ifndef PLAYINGSTATE_HPP_INCLUDED
#define PLAYINGSTATE_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>

#include "GameState.hpp"
#include "Chunk.hpp"
#include "Sprite.hpp"
#include "Constants.hpp"

#include <memory>

class Text;
class Player;

class PlayingState : public GameState
{
private:
    int camera_x = MAP_X/2;
    int camera_y = 0;
    int moveCameraX = 0;
    int moveCameraY = 0;
    bool generated = false;

    std::vector<std::unique_ptr<Chunk>> chunks;
    std::unique_ptr<Player> player;
    std::unique_ptr<Text> fpsText;

    sf::Clock clock;
    sf::Texture tileMap;
    sf::RenderStates states;

public:
    PlayingState(Game*);
    ~PlayingState();
    void loadResources();
    void loadMap();
    void update() override;
    void draw() override;
    void fillVertexArray(sf::VertexArray& vertices, std::vector<Tile*>& renderTiles);
};

#endif // PLAYINGSTATE_HPP_INCLUDED

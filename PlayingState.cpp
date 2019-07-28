#include "PlayingState.hpp"
#include "Tile.hpp"
#include "Text.hpp"
#include "Utilities.hpp"
#include "NoiseGenerator.hpp"
#include "Constants.hpp"

#include <thread>


PlayingState::PlayingState(Game* game_)
{
    game = game_;
    loadResources();
    loadMap();
}

PlayingState::~PlayingState()
{
    for (Chunk* chunk : chunks)
    {
        delete chunk;
    }
    delete fpsText;
}

void PlayingState::loadResources()
{
    fpsText = new Text(game->miscFont, "", 24, sf::Color::White, sf::Vector2f(WINDOW_X-100, 0));

    dirtTexture.loadFromFile("img/dirt.png");
    dirtSprite.setTexture(dirtTexture);

    grassTexture.loadFromFile("img/grass.png");
    grassSprite.setTexture(grassTexture);

    stoneTexture.loadFromFile("img/stone.png");
    stoneSprite.setTexture(stoneTexture);
}

void PlayingState::loadMap()
{
    std::thread t1(&PlayingState::generateMap, this);
    Text loadingText = Text(game->miscFont, "Generating World...", 48, sf::Color::White, sf::Vector2f(WINDOW_X/2-200, WINDOW_Y/2-100));
    while (!generated)
    {
        sf::Event event;
        while (game->window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                game->window.close();
        }
        game->window.clear();
        loadingText.draw(game->window);
        game->window.display();
    }
    t1.join();
}

void PlayingState::update()
{
    sf::Event event;
    while (game->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            game->window.close();
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                moveCameraX = SCROLL_SPEED;
            else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
                moveCameraX = -SCROLL_SPEED;
            else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                moveCameraY = SCROLL_SPEED;
            else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
                moveCameraY = -SCROLL_SPEED;
        } else if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                moveCameraX = 0;
            else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
                moveCameraX = 0;
            else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S)
                moveCameraY = 0;
            else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W)
                moveCameraY = 0;
        }
    }

    if ((camera_x + moveCameraX) < MAP_X - WINDOW_X && (camera_x + moveCameraX) > 0)
        camera_x += moveCameraX;
    if ((camera_y + moveCameraY) < MAP_Y - WINDOW_Y  && (camera_y + moveCameraY) > 0)
        camera_y += moveCameraY;
}

void PlayingState::generateMap()
{
    srand(time(NULL));
    NoiseGenerator generator;
    generator.generatePerlinNoise(8);
    std::vector<Tile*> tiles;
    for (int x=0; x<MAP_X; x+=BLOCK_SIZE)
    {
        int height = generator.perlinNoise[x/BLOCK_SIZE] * MAP_Y;
        int remainder = height % BLOCK_SIZE;
        height = height + BLOCK_SIZE - remainder;

        int blockDepth = 0;
        // Randomise block depths
        int stoneBlockDepth = randomInt(6,7);
        // Determine block types
        for (int y=height; y<MAP_Y; y+=BLOCK_SIZE)
        {
            int tileType = 1; // Grass
            if (blockDepth > 0 && blockDepth < stoneBlockDepth) tileType = 2; // Dirt
            else if (blockDepth >= stoneBlockDepth) tileType = 3; // Stone
            Tile* newTile = new Tile(x, y, blockDepth, tileType, 50);
            tiles.push_back(newTile);
            blockDepth++;
        }
    }

    for (int y=0; y<MAP_Y; y+=CHUNK_SIZE*BLOCK_SIZE)
    {
        for (int x=0; x<MAP_X; x+=CHUNK_SIZE*BLOCK_SIZE)
        {
            Chunk* newChunk = new Chunk(x, y, CHUNK_SIZE*BLOCK_SIZE, CHUNK_SIZE*BLOCK_SIZE);
            for (int i=0; i<tiles.size(); i++)
            {
                if (newChunk->rect.contains(tiles[i]->x + BLOCK_SIZE/2, tiles[i]->y + BLOCK_SIZE/2))
                {
                    newChunk->tiles.push_back(tiles[i]);
                }
            }
            chunks.push_back(newChunk);
        }
    }
    generated = true;
}

void PlayingState::draw()
{
    game->window.clear(sf::Color(135,206,250));
    sf::IntRect shiftedWindowRect(0 + camera_x, 0 + camera_y, WINDOW_X, WINDOW_Y);
    int sf = 64;
    int shadowDepth = 2;
    for (Chunk* chunk : chunks)
    {
        if (shiftedWindowRect.intersects(chunk->rect))
        {
            for (Tile* tile : chunk->tiles)
            {
                int brightness = 255;
                if (tile->depth > shadowDepth)
                {
                    brightness = 255 - ((tile->depth-shadowDepth)*sf);
                    if (brightness < 0) brightness = 0;
                    else if (brightness > 255) brightness = 255;
                }

                switch(tile->type)
                {
                case 1:
                    grassSprite.setPosition(tile->x - camera_x, tile->y - camera_y);
                    game->window.draw(grassSprite);
                    break;
                case 2:
                    dirtSprite.setColor(sf::Color(brightness, brightness, brightness));
                    dirtSprite.setPosition(tile->x - camera_x, tile->y - camera_y);
                    game->window.draw(dirtSprite);
                    break;
                case 3:
                    stoneSprite.setColor(sf::Color(brightness, brightness, brightness));
                    stoneSprite.setPosition(tile->x - camera_x, tile->y - camera_y);
                    game->window.draw(stoneSprite);
                    break;
                default:
                    break;
                }
            }
        }
    }
    float currentTime = clock.restart().asSeconds();
    int fps = 1 / currentTime;
    fpsText->setText("FPS: " + std::to_string(fps));
    fpsText->draw(game->window);
    game->window.display();
}

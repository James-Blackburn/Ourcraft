#include "PlayingState.hpp"
#include "Tile.hpp"
#include "Text.hpp"
#include "Utilities.hpp"
#include "Constants.hpp"
#include "Player.hpp"
#include "MapGenerator.hpp"

#include <thread>
#include <iostream>


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
    delete player;
}

void PlayingState::loadResources()
{
    fpsText = new Text(game->miscFont, "", 24, sf::Color::White, sf::Vector2f(WINDOW_X-100, 0));
    player = new Player();

    dirtSprite.loadFromFile("img/dirt.png");
    grassSprite.loadFromFile("img/grass.png");
    stoneSprite.loadFromFile("img/stone.png");
    waterSprite.loadFromFile("img/water.png");
    sandSprite.loadFromFile("img/sand.png");
    treeWoodSprite.loadFromFile("img/treeWood.png");
    leafSprite.loadFromFile("img/leaf.png");
    emptySprite.loadFromFile("img/empty.png");
}

void PlayingState::loadMap()
{
    srand(time(NULL));
    bool generated = false;
    std::thread t1(&MapGenerator::generateMap, MapGenerator(), std::ref(chunks), player, std::ref(generated));
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
            if (event.key.code == sf::Keyboard::Escape)
                game->window.close();
            else if (event.key.code == sf::Keyboard::Space)
                player->moveY = JUMP_SPEED;
            else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
            {
                player->moveX = MOVE_SPEED;
                player->changeState(PlayerState::WALKING_RIGHT);
            }
            else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
            {
                player->moveX = -MOVE_SPEED;
                player->changeState(PlayerState::WALKING_LEFT);
            }
        } else if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D
                || event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
            {
                player->moveX = 0;
                player->changeState(PlayerState::STANDING);
            }
        }
    }

    player->update(chunks);

    // Move camera
    camera_x = player->x - WINDOW_X / 2;
    camera_y = player->y - WINDOW_Y / 2;
}

void PlayingState::draw()
{
    game->window.clear(sf::Color(135,206,250));
    sf::IntRect shiftedWindowRect(camera_x, camera_y, WINDOW_X, WINDOW_Y);

    for (Chunk* chunk : chunks)
    {
        // Only draw chunks that intersect with player view
        if (shiftedWindowRect.intersects(chunk->rect))
        {
            for (Tile* tile : chunk->tiles)
            {
                // Apply Frustrum Culling
                if (tile->x <= 0 || tile->x >= camera_x+WINDOW_X
                    || tile->y <= 0 || tile->y >= camera_y+WINDOW_Y)
                    continue;

                int brightness = MAX_TILE_BRIGHTNESS;
                sf::Sprite* tileSprite = nullptr;

                // Draw tiles
                switch(tile->type)
                {
                case TileType::GRASS:
                    tileSprite = &grassSprite;
                    break;
                case TileType::DIRT:
                    tileSprite = &dirtSprite;
                    brightness = tile->getBrightness();
                    tileSprite->setColor(sf::Color(brightness, brightness, brightness));
                    break;
                case TileType::STONE:
                    tileSprite = &stoneSprite;
                    brightness = tile->getBrightness();
                    tileSprite->setColor(sf::Color(brightness, brightness, brightness));
                    break;
                case TileType::SAND:
                    tileSprite = &sandSprite;
                    brightness = tile->getBrightness();
                    tileSprite->setColor(sf::Color(brightness, brightness, brightness));
                    break;
                case TileType::TREEWOOD:
                    tileSprite = &treeWoodSprite;
                    break;
                case TileType::LEAF:
                    tileSprite = &leafSprite;
                    break;
                case TileType::WATER:
                    tileSprite = &waterSprite;
                    break;
                }

                tileSprite->setPosition(tile->x - camera_x, tile->y - camera_y);
                game->window.draw(*tileSprite);
            }
        }
    }


    player->sprite->setPosition(player->x - camera_x, player->y - camera_y);
    game->window.draw(*player->sprite);

    // Calculate FPS
    float currentTime = clock.restart().asSeconds();
    int fps = 1 / currentTime;
    fpsText->setText("FPS: " + std::to_string(fps));
    fpsText->draw(game->window);
    game->window.display();
}

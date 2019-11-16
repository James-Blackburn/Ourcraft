#include "PlayingState.hpp"
#include "Tile.hpp"
#include "Text.hpp"
#include "Utilities.hpp"
#include "Constants.hpp"
#include "Player.hpp"
#include "MapGenerator.hpp"
#include "Game.hpp"

#include <thread>
#include <string>


PlayingState::PlayingState(Game* game_)
{
    game = game_;
    loadResources();
    loadMap();
}

PlayingState::~PlayingState() = default;

void PlayingState::loadResources()
{
    fpsText = std::make_unique<Text>(game->miscFont, "", 24, sf::Color::White, sf::Vector2f(game->windowX-100, 0));
    player = std::make_unique<Player>();
    tileMap.loadFromFile("img/tilemap.png");
    states.texture = &tileMap;
}

void PlayingState::loadMap()
{
    Text loadingText = Text(game->miscFont, "Generating World...", 48, sf::Color::White, sf::Vector2f(game->windowX / 2, game->windowY / 2 - 100));
    Text logText = Text(game->miscFont, "", 24, sf::Color::White, sf::Vector2f(game->windowX / 2, game->windowY / 2));

    bool generated = false;
    std::string currentText = "";
    std::thread t1(&MapGenerator::generateMap, MapGenerator(), std::ref(chunks), std::ref(player), std::ref(generated), std::ref(currentText));

    while (!generated)
    {
        logText.setText(currentText);
        sf::Event event;
        while (game->window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                game->window.close();
        }
        game->window.clear();
        loadingText.draw(game->window);
        logText.draw(game->window);
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
        else if (event.type == sf::Event::KeyPressed) // Key pressed
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
        } else if (event.type == sf::Event::KeyReleased) // Key released
        {
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D
                || event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
            {
                player->moveX = 0;
                player->changeState(PlayerState::STANDING);
            }
        } else if (event.type == sf::Event::MouseButtonPressed) // Mouse button pressed
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mpos = sf::Vector2i(sf::Mouse::getPosition(game->window));
                player->mine(mpos.x + camera_x, mpos.y + camera_y, chunks);
            }
        }
    }

    player->update(chunks);

    // Move camera
    camera_x = player->x - game->windowX / 2;
    camera_y = player->y - game->windowY / 2;
}

void PlayingState::fillVertexArray(sf::VertexArray& vertices, std::vector<Tile*>& renderTiles)
{
    for (int i=0; i<renderTiles.size(); i++)
    {
        Tile* tile = renderTiles[i];
        sf::Vertex* quad = &vertices[i * 4];

        // Set quad positions
        quad[0].position =  sf::Vector2f(tile->x - camera_x, tile->y - camera_y);
        quad[1].position = sf::Vector2f(tile->x + BLOCK_SIZE - camera_x, tile->y - camera_y);
        quad[2].position =  sf::Vector2f(tile->x + BLOCK_SIZE - camera_x, tile->y + BLOCK_SIZE - camera_y);
        quad[3].position =  sf::Vector2f(tile->x - camera_x, tile->y + BLOCK_SIZE - camera_y);

        // Get quad colours
        const int quad0Colour = tile->getBrightness(player.get(), tile->x, tile->y);
        const int quad1Colour = tile->getBrightness(player.get(), tile->x + BLOCK_SIZE, tile->y);
        const int quad2Colour = tile->getBrightness(player.get(), tile->x + BLOCK_SIZE, tile->y + BLOCK_SIZE);
        const int quad3Colour = tile->getBrightness(player.get(), tile->x, tile->y + BLOCK_SIZE);

        // Set quad colours
        quad[0].color = sf::Color(quad0Colour, quad0Colour, quad0Colour);
        quad[1].color = sf::Color(quad1Colour, quad1Colour, quad1Colour);
        quad[2].color = sf::Color(quad2Colour, quad2Colour, quad2Colour);
        quad[3].color = sf::Color(quad3Colour, quad3Colour, quad3Colour);

        // Set quad texture coordinates
        quad[0].texCoords = sf::Vector2f(BLOCK_SIZE * tile->type, 0);
        quad[1].texCoords = sf::Vector2f(BLOCK_SIZE * tile->type + BLOCK_SIZE, 0);
        quad[2].texCoords = sf::Vector2f(BLOCK_SIZE * tile->type + BLOCK_SIZE, BLOCK_SIZE);
        quad[3].texCoords = sf::Vector2f(BLOCK_SIZE * tile->type, BLOCK_SIZE);
    }
}

void PlayingState::draw()
{
    game->window.clear();
    sf::IntRect shiftedWindowRect(camera_x, camera_y, game->windowX, game->windowY);

    std::vector<Tile*> renderTiles;
    renderTiles.reserve(static_cast<int>((game->windowX / BLOCK_SIZE) * (game->windowY / BLOCK_SIZE)));
    for (std::unique_ptr<Chunk>& chunk : chunks)
    {
        // Only draw chunks that intersect with player view
        if (shiftedWindowRect.intersects(chunk->rect))
        {
            for (std::shared_ptr<Tile>& tile : chunk->tiles)
            {
                // Apply Frustrum Culling
                if (tile->x <= 0 || tile->x >= camera_x+game->windowX || tile->y <= 0 || tile->y >= camera_y+game->windowY)
                    continue;

                // Only bother to render if visible
                int xDistance = std::abs(player->x - tile->x);
                int yDistance = std::abs(player->y - tile->y);
                int distance =  std::sqrt((xDistance * xDistance) + (yDistance * yDistance));

                if (distance < (MAX_TILE_BRIGHTNESS * 2))
                    renderTiles.push_back(tile.get());
            }
        }
    }

    // Create, fill and draw Vertex Array
    sf::VertexArray vertices(sf::Quads, renderTiles.size() * 4);
    fillVertexArray(vertices, renderTiles);
    game->window.draw(vertices, states);

    player->sprite->setPosition(player->x - camera_x, player->y - camera_y);
    game->window.draw(*player->sprite);

    // Calculate FPS
    float currentTime = clock.restart().asSeconds();
    int fps = 1 / currentTime;
    fpsText->setText("FPS: " + std::to_string(fps));
    fpsText->draw(game->window);
    game->window.display();
}

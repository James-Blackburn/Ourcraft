#include "PlayingState.hpp"
#include "Tile.hpp"
#include "Text.hpp"
#include "Utilities.hpp"
#include "NoiseGenerator.hpp"
#include "Constants.hpp"
#include "Player.hpp"

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
    player = new Player;
    player->x = camera_x;
    player->y = camera_y;

    playerTexture.loadFromFile("img/player.png");
    player->setTexture(playerTexture);

    dirtTexture.loadFromFile("img/dirt.png");
    dirtSprite.setTexture(dirtTexture);

    grassTexture.loadFromFile("img/grass.png");
    grassSprite.setTexture(grassTexture);

    stoneTexture.loadFromFile("img/stone.png");
    stoneSprite.setTexture(stoneTexture);

    emptyTexture.loadFromFile("img/empty.png");
    emptySprite.setTexture(emptyTexture);
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

void PlayingState::update()
{
    sf::Event event;
    while (game->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            game->window.close();
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Space)
                player->moveY = 8;
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                player->moveX = MOVE_SPEED;
            else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
                player->moveX = -MOVE_SPEED;
        } else if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D)
                player->moveX = 0;
            else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A)
                player->moveX = 0;
        }
    }

    // Move player according to gravity
    int playerFutureY;

    if (player->moveY)
    {
        player->y += -player->moveY;
        player->moveY--;
        playerFutureY = player->y;
    }
    else
        playerFutureY = player->y + GRAVITY;

    for (Chunk* chunk : chunks)
    {
        if (chunk->rect.contains(player->x, playerFutureY))
        {
            Tile* closestTile = nullptr;
            for (Tile* tile : chunk->tiles)
            {
                if (tile->x <= player->x + PLAYER_WIDTH/2 && tile->x + BLOCK_SIZE >= player->x + PLAYER_WIDTH/2
                    && tile->y >= player->y + PLAYER_HEIGHT && tile->y <= playerFutureY + PLAYER_HEIGHT)
                {
                    if (closestTile == nullptr)
                        closestTile = tile;
                    else if (tile->y < closestTile->y)
                        closestTile = tile;
                }
            }
            if (closestTile != nullptr)
                playerFutureY = closestTile->y - PLAYER_HEIGHT;
        }
    }

    
    int playerFutureX = player->x + player->moveX;
	Chunk* collisionChunk = nullptr;
	for (Chunk* chunk : chunks)
	{
		if (chunk->rect.contains(playerFutureX, player->y))
		{
			collisionChunk = chunk;
			break;
		}
	}
			
	// Move player right
    if (playerFutureX > player->x)
    {
		Tile* closestTile = nullptr;
		for (Tile* tile : collisionChunk->tiles)
		{
			if (tile->y < player->y + PLAYER_HEIGHT && tile->y > player->y
				&& tile->x >= player->x + PLAYER_WIDTH && tile->x <= playerFutureX + PLAYER_WIDTH)
			{
				if (closestTile == nullptr)
					closestTile = tile;
				else if (tile->x < closestTile->x)
					closestTile = tile;
			}
		}
		if (closestTile != nullptr)
			playerFutureX = closestTile->x - PLAYER_WIDTH;
    } 
	// Move player left
	else if (playerFutureX < player->x)
    { 
		Tile* closestTile = nullptr;
		for (Tile* tile : collisionChunk->tiles)
		{
			if (tile->y < player->y + PLAYER_HEIGHT && tile->y > player->y
				&& tile->x + BLOCK_SIZE <= player->x && tile->x >= playerFutureX - BLOCK_SIZE)
			{
				if (closestTile == nullptr)
					closestTile = tile;
				else if (tile->x > closestTile->x)
					closestTile = tile;
			}
		}
		if (closestTile != nullptr)
			playerFutureX = closestTile->x + BLOCK_SIZE;
    }

	// Set new player position
    player->y = playerFutureY;
    player->x = playerFutureX;

    // Move camera
    camera_x = player->x - (WINDOW_X / 2);
    camera_y = player->y - (WINDOW_Y / 2);
}

void PlayingState::draw()
{
    game->window.clear(sf::Color(135,206,250));
    sf::IntRect shiftedWindowRect(0 + camera_x, 0 + camera_y, WINDOW_X, WINDOW_Y);

    for (Chunk* chunk : chunks)
    {
        if (shiftedWindowRect.intersects(chunk->rect))
        {
            for (Tile* tile : chunk->tiles)
            {
                int brightness = MAX_TILE_BRIGHTNESS;
                if (tile->depth > SHADOW_DEPTH)
                {
                    brightness = MAX_TILE_BRIGHTNESS - ((tile->depth-SHADOW_DEPTH)*SHADOW_SCALING_FACTOR);
                    if (brightness < 0)
                    {
                        emptySprite.setPosition(tile->x - camera_x, tile->y - camera_y);
                        game->window.draw(emptySprite);
                        continue;
                    }
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
                }
            }
        }
    }

    player->setPosition(player->x - camera_x, player->y - camera_y);
    game->window.draw(*player);

    // Calculate FPS
    float currentTime = clock.restart().asSeconds();
    int fps = 1 / currentTime;
    fpsText->setText("FPS: " + std::to_string(fps));
    fpsText->draw(game->window);
    game->window.display();
}

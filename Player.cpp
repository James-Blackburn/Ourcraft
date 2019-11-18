#include "Player.hpp"
#include "Chunk.hpp"
#include "Tile.hpp"
#include "Constants.hpp"

#include <iostream>

Player::Player()
{
    frameTimer = std::chrono::system_clock::now();

    walkingRightFrames.push_back(std::unique_ptr<Sprite>(new Sprite("img/player/playerRight1.png")));
    walkingRightFrames.push_back(std::unique_ptr<Sprite>(new Sprite("img/player/playerRight2.png")));
    walkingRightFrames.push_back(std::unique_ptr<Sprite>(new Sprite("img/player/playerRight3.png")));

    walkingLeftFrames.push_back(std::unique_ptr<Sprite>(new Sprite("img/player/playerLeft1.png")));
    walkingLeftFrames.push_back(std::unique_ptr<Sprite>(new Sprite("img/player/playerLeft2.png")));
    walkingLeftFrames.push_back(std::unique_ptr<Sprite>(new Sprite("img/player/playerLeft3.png")));

    standingSprite.loadFromFile("img/player/playerStanding1.png");
}

void Player::update(std::vector<std::unique_ptr<Chunk>>& chunks)
{
    handleStates();
    handleCollisions(chunks);
}

void Player::handleStates()
{
    switch(state)
    {
        case PlayerState::STANDING:
            sprite = &standingSprite;
            break;
        case PlayerState::WALKING_LEFT:
            animatePlayer(walkingLeftFrames);
            break;
        case PlayerState::WALKING_RIGHT:
            animatePlayer(walkingRightFrames);
            break;
    }
}

void Player::animatePlayer(std::vector<std::unique_ptr<Sprite>>& frames)
{
    std::chrono::time_point<std::chrono::system_clock> currentTime =
        std::chrono::system_clock::now();
    std::chrono::duration<double> elapsedSeconds = currentTime - frameTimer;
    if (elapsedSeconds.count() > frameChangeTime)
    {
        frameTimer = std::chrono::system_clock::now();
        if (currentFrame >= frames.size() -1)
        {
            currentFrame = 0;
        } else
            currentFrame++;
    }
    sprite = frames[currentFrame].get();
}

void Player::changeState(int newState)
{
    if (newState != state)
    {
        state = newState;
        currentFrame = 0;
        frameTimer = std::chrono::system_clock::now();
    }
}

void Player::mine(int mposX, int mposY, std::vector<std::unique_ptr<Chunk>>& chunks)
{
    for (std::unique_ptr<Chunk>& chunk : chunks)
    {
        if (chunk->rect.contains(mposX, mposY))
        {
            int tileIndex = 0;
            for (int tileIndex=0; tileIndex < chunk->tiles.size(); tileIndex++)
            {
                if (chunk->tiles[tileIndex]->rect.contains(mposX, mposY))
                {
                    chunk->removeTile(tileIndex);
                    break;
                }
            }
        }
    }
}

void Player::handleCollisions(std::vector<std::unique_ptr<Chunk>>& chunks)
{
    // Move player according to gravity
    int playerFutureY;
    int playerFutureX = x + moveX;

    if (moveY)
    {
        playerFutureY = y - moveY;
        moveY--;
    }
    else
        playerFutureY = y + GRAVITY;

    sf::IntRect playerFutureRectY(x, playerFutureY, PLAYER_WIDTH, PLAYER_HEIGHT);

    for (std::unique_ptr<Chunk>& chunk : chunks)
    {
        if (chunk->rect.intersects(playerFutureRectY))
        {
            Tile* closestTile = nullptr;
            int closestTileIndex = 0;
            for (std::shared_ptr<Tile>& tile : chunk->tiles)
            {
                // Do not collide with water or trees
                if (tile->type == TileType::WATER || tile->type == TileType::TREEWOOD || tile->type == TileType::LEAF)
                    continue;

                if (playerFutureRectY.intersects(tile->rect))
                {
                    if (closestTile == nullptr)
                        closestTile = tile.get();
                    else if (tile->y < closestTile->y)
                        closestTile = tile.get();
                }
            }
            if (closestTile != nullptr)
                playerFutureY = closestTile->y - PLAYER_HEIGHT;
        }
    }

    if (playerFutureY > 0 && playerFutureY + PLAYER_HEIGHT < MAP_Y)
        y = playerFutureY;

    std::vector<Tile*> collisionTiles;
    sf::IntRect playerFutureRectX(playerFutureX, y, PLAYER_WIDTH,
                                 PLAYER_HEIGHT);
    for (std::unique_ptr<Chunk>& chunk : chunks)
    {
        if (chunk->rect.intersects(playerFutureRectX))
        {
            collisionTiles.reserve(chunk->tiles.size());
            for (std::shared_ptr<Tile>& tile : chunk->tiles)
            {
                collisionTiles.push_back(tile.get());
            }
        }
    }


    // Move player right
    if (playerFutureX > x)
    {
        Tile* closestTile = nullptr;
        for (Tile* tile : collisionTiles)
        {
            // Do not collide with water or trees
            if (tile->type == TileType::WATER || tile->type == TileType::TREEWOOD || tile->type == TileType::LEAF)
                continue;

            if (playerFutureRectX.intersects(tile->rect))
            {
                if (closestTile == nullptr)
                    closestTile = tile;
                else if (tile->x < closestTile->x)
                    closestTile = tile;
            }
        }
        if (closestTile != nullptr)
        {
            playerFutureX = closestTile->x - PLAYER_WIDTH;
        }
    }
    else if (playerFutureX < x)
    { // Move player left
        state = PlayerState::WALKING_LEFT;
        Tile* closestTile = nullptr;
        for (Tile* tile : collisionTiles)
        {
            // Do not collide with water or trees
            if (tile->type == TileType::WATER || tile->type == TileType::TREEWOOD || tile->type == TileType::LEAF)
                continue;

            if (playerFutureRectX.intersects(tile->rect))
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

    if (playerFutureX > 0 && playerFutureX + PLAYER_WIDTH < MAP_X)
        x = playerFutureX;
}

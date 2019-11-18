#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include <memory>

#include "Sprite.hpp"

struct Chunk;

enum PlayerState
{
    STANDING, WALKING_LEFT, WALKING_RIGHT,
    JUMPING, SWIMMING
};

class Player
{
public:

    int x = 0;
    int y = 0;
    int moveY = 0;
    int moveX = 0;

    Sprite* sprite;

    Player();
    ~Player() = default;

    void update(std::vector<std::unique_ptr<Chunk>>& chunks);
    void mine(int mposX, int mposY, std::vector<std::unique_ptr<Chunk>>& chunks);
    void changeState(int newState);

private:

    std::vector<std::unique_ptr<Sprite>> walkingRightFrames;
    std::vector<std::unique_ptr<Sprite>> walkingLeftFrames;
    Sprite standingSprite;

    int state = PlayerState::STANDING;
    int currentFrame = 0;
    std::chrono::time_point<std::chrono::system_clock> frameTimer;
    double frameChangeTime = 0.10f;

    void animatePlayer(std::vector<std::unique_ptr<Sprite>>& frames);
    void handleCollisions(std::vector<std::unique_ptr<Chunk>>& chunks);
    void handleStates();
};

#endif // PLAYER_HPP_INCLUDED

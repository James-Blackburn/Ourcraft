#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

#include "Sprite.hpp"

class Chunk;

enum PlayerState
{
    STANDING, WALKING_LEFT, WALKING_RIGHT, JUMPING, SWIMMING
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
    ~Player();

    void update(std::vector<Chunk*>& chunks);
    void changeState(int newState);

private:

    std::vector<Sprite*> walkingRightFrames;
    std::vector<Sprite*> walkingLeftFrames;
    Sprite standingSprite;

    int state = PlayerState::STANDING;
    int currentFrame = 0;
    std::chrono::time_point<std::chrono::system_clock> frameTimer;
    double frameChangeTime = 0.10f;

    void handleCollisions(std::vector<Chunk*>& chunks);
    void handleStates();
    void animatePlayer(std::vector<Sprite*>& frames);
};

#endif // PLAYER_HPP_INCLUDED

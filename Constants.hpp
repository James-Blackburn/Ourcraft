#ifndef CONSTANTS_HPP_INCLUDED
#define CONSTANTS_HPP_INCLUDED

const int WINDOW_X = 800;
const int WINDOW_Y = 480;
const int MAP_X = 5120;
const int MAP_Y = 1024;
const int CHUNK_SIZE = 4;
const int BLOCK_SIZE = 16;
const int MAP_BLOCK_WIDTH = MAP_X / BLOCK_SIZE;
const int MAP_BLOCK_HEIGHT = MAP_Y / BLOCK_SIZE;
const int SEED = 1;
const int MOVE_SPEED = 4;
const int GRAVITY = 8;
const int SHADOW_SCALING_FACTOR = 64;
const int SHADOW_DEPTH = 2;
const int MAX_TILE_BRIGHTNESS = 255;
const int PLAYER_WIDTH = 8;
const int PLAYER_HEIGHT = 32;

#endif // CONSTANTS_HPP_INCLUDED

#ifndef CONSTANTS_HPP_INCLUDED
#define CONSTANTS_HPP_INCLUDED

#define MAP_X                   10240
#define MAP_Y                   2048
#define CHUNK_SIZE              8
#define BLOCK_SIZE              16
#define MAP_BLOCK_WIDTH         (MAP_X / BLOCK_SIZE)
#define MAP_BLOCK_HEIGHT        (MAP_Y / BLOCK_SIZE)
#define MOVE_SPEED              4
#define JUMP_SPEED              8
#define GRAVITY                 8
#define MAX_TILE_BRIGHTNESS     255
#define PLAYER_WIDTH            16
#define PLAYER_HEIGHT           64
#define TREE_CHANCE             0.10

#endif // CONSTANTS_HPP_INCLUDED

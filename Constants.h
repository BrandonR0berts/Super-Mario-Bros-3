#pragma once

#define SCREEN_WIDTH  512
#define SCREEN_HEIGHT 416

#define SPRITE_RES 32

#define SCREEN_HEIGHT_GRID (SCREEN_HEIGHT / SPRITE_RES)
#define SCREEN_WIDTH_GRID  (SCREEN_WIDTH  / SPRITE_RES)

const float CHARACTER_MOVEMENT_SPEED        =  1.0;
const float CHARACTER_JUMP_FORCE_DECREMENT  =  5.0;
const float CHARACTER_INITIAL_JUMP_FORCE    = -7.0;

const float CHARACTER_GRAVITY               =  11.0;
const float GRAVITY                         =  6.0;
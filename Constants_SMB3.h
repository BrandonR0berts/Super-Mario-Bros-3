#ifndef _CONSTANTS_SMB3_H_
#define _CONSTANTS_SMB3_H_

// Actual window dimensions in pixels
#define SCREEN_WIDTH_SMB3 768
#define SCREEN_HEIGHT_SMB3 768

// Rendering boundary 
#define BACKGROUND_SPRITE_RENDER_WIDTH  18
#define BACKGROUND_SPRITE_RENDER_HEIGHT 12

// Used for spawning/despawning enemies
#define LEVEL_BOUNDING_AREA_WIDTH 17
#define LEVEL_BOUNDING_AREA_HEIGHT 14

#define HALF_LEVEL_BOUNDING_AREA_HEIGHT (LEVEL_BOUNDING_AREA_HEIGHT / 2)
#define HALF_LEVEL_BOUNDING_AREA_WIDTH (LEVEL_BOUNDING_AREA_WIDTH / 2)

// Pixel definition of sprites
#define RESOLUTION_OF_SPRITES 48

// Capping size of levels
#define MAX_LEVEL_WIDTH 255
#define MAX_LEVEL_HEIGHT 63

#define WORLD_MAP_PLAYER_MOVEMENT_SPEED 3.0f

// So that when files load in they dont go infinitally
#define FAILSAFE_MAX_COUNT_FILE_LOADING_LOOPS 300

#define MOVEMENT_DELAY_WORLD_MAP 0.05f

#define COLLECTABLE_MOVEMENT_SPEED 1.5f

#define SCREEN_WIDTH_GRID_SMB3  (SCREEN_WIDTH_SMB3 / RESOLUTION_OF_SPRITES)
#define SCREEN_HEIGHT_GRID_SMB3 (SCREEN_HEIGHT_SMB3 / RESOLUTION_OF_SPRITES)

#define HALF_SCREEN_WIDTH_GRID_SMB3   (SCREEN_WIDTH_GRID_SMB3 / 2)
#define HALF_SCREEN_HEIGHT_GRID_SMB3  (SCREEN_HEIGHT_GRID_SMB3 / 2)

#define PLAYABLE_SCREEN_AREA_WIDTH (BACKGROUND_SPRITE_RENDER_WIDTH)
#define PLAYABLE_SCREEN_AREA_HEIGHT ((BACKGROUND_SPRITE_RENDER_HEIGHT / 4.0f) * 3.0f)

#endif
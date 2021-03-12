#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include "Game_Maths.h"

class LevelMap;

// ---------------------------------------------------------------------------- //

enum PlayerMovementData
{
	NONE_SMB1           = 0,
	WALKING_LEFT_SMB1   = 1 << 0,
	WALKING_RIGHT_SMB1  = 1 << 1,
	JUMPING_SMB1        = 1 << 2,
	DEAD_SMB1           = 1 << 3
};

// ---------------------------------------------------------------------------- //

class Texture2D;

class Character final
{
public:
	Character() = delete;
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startingPosition, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight, LevelMap* levelMap, const float collisionCircleRadius);
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startingPosition, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight, LevelMap* levelMap, const Vector2D collisionBox);
	~Character();

	void          Render();
	void          Update(float deltaTime, SDL_Event e);

	void          SetPosition(Vector2D newPos);
	Vector2D      GetPosition()        const { return mPosition; }

	double        GetCollisionRadius() const { return mCollisionRadius; }
	Vector2D      GetCollisionBox()    const { return mCollisionBox; }

protected:
	void          Jump();
	void          WalkRight();
	void          WalkLeft();

	void          HandleInput(SDL_Event e);
	void          UpdatePhysics(const float deltaTime);

	// General data
	SDL_Renderer*      mRenderer;
	Vector2D           mPosition;
	Texture2D*         mTexture;
	float              mJumpForce;

	// Collision Data
	Vector2D           mCollisionBox;
	double             mCollisionRadius;

	Vector2D           mVelocity;

	const unsigned int kSpritesOnWidth;
	const unsigned int kSpritesOnHeight;

	LevelMap*          mLevelMap;

	unsigned int       mPlayerMovementData;
};

#endif _CHARACTER_H_
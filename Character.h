#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include "Game_Maths.h"

#include "AudioPlayer.h"

class LevelMap;

#define DEATH_ANIMATION_SPEED 3.0

// ---------------------------------------------------------------------------- //

enum PlayerMovementData
{
	NONE_SMB1           = 0,
	WALKING_LEFT_SMB1   = 1 << 0,
	WALKING_RIGHT_SMB1  = 1 << 1,
	JUMPING_SMB1        = 1 << 2,
	DEAD_SMB1           = 1 << 3,
	WAS_FACING_RIGHT    = 1 << 4
};

// ---------------------------------------------------------------------------- //

class Texture2D;

class Character final
{
public:
	Character() = delete;
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startingPosition, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight, LevelMap* levelMap, const float collisionCircleRadius, const float timePerFrame, Audio_Player* AudioPlayerRef);
	Character(SDL_Renderer* renderer, std::string imagePath, Vector2D startingPosition, const unsigned int spritesOnWidth, const unsigned int spritesOnHeight, LevelMap* levelMap, const Vector2D collisionBox, const float timePerFrame, Audio_Player* AudioPlayerRef);
	~Character();

	void          Render();
	void          Update(float deltaTime, SDL_Event e);

	void          SetPosition(Vector2D newPos);
	Vector2D      GetPosition()        const { return mPosition; }

	double        GetCollisionRadius() const { return mCollisionRadius; }
	Vector2D      GetCollisionBox()    const { return mCollisionBox; }

	Vector2D      GetCurrentVelocity()    const { return mVelocity; }

	void          SetHasBeenHit();

	void          AddToScore()              { mCoinCount++; }
	unsigned int  GetCoinCount()      const { return mCoinCount; }

	void          DecreaseLifeCount()       { mLifeCount--; }
	int           GetLifeCount()      const { return mLifeCount; };

	void          SetHitHead()              { mVelocity.y = 2.0f; }

	bool          GetIsAlive()		  const { return mIsAlive; }

protected:
	void          Jump();

	void          HandleInput(SDL_Event e);
	void          HandleCollisions(const float deltaTime);
	void          ApplyMovement(const float deltaTime);
	void		  CheckForLooping();

	void          UpdateDeathAnimationMovement(const float deltaTime);

	void          HandleAnimations(const float deltaTime);

	void		  CalculateSpriteData(SDL_Renderer* renderer, std::string filePath);

	void          AddGravity(const float deltaTime);

	void          RespawnPlayer();

	// General data
	SDL_Renderer*      mRenderer;
	Vector2D           mPosition;
	Texture2D*         mTexture;
	float              mJumpForce;

	// Collision Data
	Vector2D           mCollisionBox;
	double             mCollisionRadius;

	Vector2D           mVelocity;

	Vector2D           mDeathPosition;
	Vector2D           mSpawnPosition;

	const unsigned int kSpritesOnWidth;
	const unsigned int kSpritesOnHeight;

	unsigned int       mCurrentSpriteID;
	unsigned int       mEndSpriteID;
	unsigned int       mStartSpriteID;

	const float        kTimePerFrame;
	float              mTimeRemainingPerFrame;

	unsigned int       mSingleSpriteWidth;
	unsigned int       mSingleSpriteHeight;

	LevelMap*          mLevelMap;

	unsigned int       mPlayerMovementData;

	unsigned int       mCoinCount;

	int                mLifeCount;

	SDL_Rect*          mSourceRect;
	SDL_Rect*          mDestRect;

	Audio_Player*      mAudioPlayerRef;

	bool               mUsingCollisionBox;
	bool               mIsAlive;
	bool               mHasCompletedDeathBounce;
};

#endif _CHARACTER_H_
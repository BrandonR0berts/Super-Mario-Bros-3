#ifndef _PLAYABLE_CHARACTER_H
#define _PLAYABLE_CHARACTER_H

#include "Commons_SMB3.h"
#include "Texture2D.h"

#include "Subject.h"

class InteractableLayer;
class ObjectLayer;

// ---------------------------------------------------------------- //

enum MovementBitField : unsigned int
{
	NONE         = 0,
	MOVING_RIGHT = 1 << 0,
	MOVING_LEFT  = 1 << 1,
	CROUCHING    = 1 << 2,

	RUNNING      = 1 << 3,
	JUMPING      = 1 << 4,
	HOLDING_JUMP = 1 << 5,
	SWIMMING     = 1 << 6,

	ENTERING_PIPE_VERTICALLY = 1 << 7
};

// ---------------------------------------------------------------- //

class PlayableCharacter final : public Subject
{
public:
	PlayableCharacter(SDL_Renderer* renderer, const char* filePathToSpriteSheet, Vector2D spawnPoint, const Vector2D levelBounds, const float timePerFrame);
	~PlayableCharacter() override;

	void Render();
	void Update(const float deltaTime, SDL_Event e, const Vector2D levelBounds, InteractableLayer* interactionLayer, ObjectLayer* objectLayer);

	const Vector2D GetRealGridPosition()     const { return mRealGridPosition; }
	const Vector2D GetScreenGridPosition()   const { return mScreenGridPosition; }

	const Vector2D GetRenderReferencePoint() const { return mRenderRefencePoint; }

	void           SpawnIntoNewArea(const Vector2D newPos, const Vector2D newLevelBounds);

	void           SetLevelOver() { mHasControl = false; }

private:
	void HandleMovementInput(SDL_Event e);

	void CalculateNewPosition(const float deltaTime, CollisionPositionalData xCollisionOccured, CollisionPositionalData yCollisionOccured);
	void CalculateInitialRenderReferencePoint();

	void CalculateScreenBoundsPosition(const Vector2D spawnPoint);

	void CalculateNewScreenPosAndRenderPos(Vector2D movementDistance, const CollisionPositionalData xCollisionOccured, const CollisionPositionalData yCollisionOccured);

	bool HandleCollisionsWithInteractionLayer(InteractableLayer* interactionLayer, const Vector2D newPos);
	MovementPrevention HandleCollisionsWithInteractionObjectLayer(ObjectLayer* objectLayer, const Vector2D newPos);

	bool CheckXCollision(const Vector2D positionToCheck1, const Vector2D positionToCheck2, InteractableLayer* interactionLayer, ObjectLayer* objectLayer);
	bool CheckYCollision(const Vector2D positionToCheck1, const Vector2D positionToCheck2, InteractableLayer* interactionLayer, ObjectLayer* objectLayer);

	void UpdatePhysics(const float deltaTime);

	void UpdateAnimations(const float deltaTime);

	void LoadInCorrectSpriteSheet();

	void UpdateAnimationsSmallMario();
	void UpdateAnimationsLargeMario();
	void UpdateAnimationsFrogMario();
	void UpdateAnimationsHammerMario();
	void UpdateAnimationsFireMario();
	void UpdateAnimationsTanookiMario();
	void UpdateAnimationsLeafMario();
	void UpdateAnimationsStarMario();

	CollisionPositionalData HandleXCollisions(const float deltaTime, InteractableLayer* interactionLayer, ObjectLayer* objectLayer);
	CollisionPositionalData HandleYCollisions(const float deltaTime, InteractableLayer* interactionLayer, ObjectLayer* objectLayer);

	Vector2D   mRealGridPosition;   // The player's position in the collision world
	Vector2D   mScreenGridPosition; // Player's screen position

	Vector2D   mRenderRefencePoint; // For when rendering the level based on where the player is in it - but not using the player's exact position

	Vector2D   mVelocity;
	Vector2D   mAcceleration;

	Vector2D   mCollisionBox; // Using grid space for the collision box, not pixel size
	Vector2D   mCollisionBoxOffset; // Not all sprites on sheets render from the bottom left so they shouldnt collide from there

	Vector2D   mLevelBounds;

	Texture2D*    mSpriteSheet;
	SDL_Renderer* mRenderer;

	unsigned int mSingleSpriteWidth;
	unsigned int mSingleSpriteHeight;

	unsigned int mNumberOfSpritesOnWidth;
	unsigned int mNumberOfSpritesOnHeight;

	unsigned int mCurrentFrame;
	unsigned int mStartFrame;
	unsigned int mEndFrame;

	float        mTimeTillNextFrame;
	const float  mTimePerFrame;

	const float  kBaseMaxHorizontalSpeed;
	const float  kMaxHorizontalSpeedOverall;
	float        mMaxHorizontalSpeed;

	const float  kMaxYVelocity;

	const float  mPSpeedAccumulatorRate;

	const float  kFrictionMultiplier;
	const float  kJumpHeldAccelerationDepreciationRate;
	float        mJumpInitialBoost;
	const float  kJumpHeldInitialBoost;
	float        mJumpHeldCurrentBoost;

	unsigned int mCurrentMovements;
	unsigned int mPriorFrameMovements;

	float        mJumpTimerLeway;

	POWER_UP_TYPE mPowerUpState;

	bool         mIsAlive;
	bool         mWasFacingRight;
	bool         mGrounded;
	bool         mHasControl;
};


#endif
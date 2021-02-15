#include "Goomba_SMB3.h"

#include <sstream>

unsigned int Goomba::mCurrentSpriteID(0);
unsigned int Goomba::mStartSpriteID(0);
unsigned int Goomba::mEndSpriteID(1);

float        Goomba::mTimeRemainingTillNextFrame(0.0f);
bool		 Goomba::mUpdatedStaticVariables(false);

// ------------------------------------------------------------- //

Goomba::Goomba(const Vector2D      spawnPosition,
	           const bool          startSpawnedInLevel,
	           SDL_Renderer* renderer,
	           const std::string   filePathToSpriteSheet,
			   const unsigned int  spritesOnWidth,
			   const unsigned int  spritesOnHeight,
			   const double        collisionBoxWidth,
			   const double        collisionBoxHeight,
		       const float		    timePerFrame,
			   const bool          canMove,
		       const bool          canJump,
			   const bool          startFacingLeft)

: EnemyObject(spawnPosition
, startSpawnedInLevel
, renderer
, filePathToSpriteSheet
, spritesOnWidth
, spritesOnHeight
, collisionBoxWidth
, collisionBoxHeight
, timePerFrame
, canMove
, canJump
, startFacingLeft)
, mTimePerFrame(timePerFrame)
, mTimerTillDespawn(1.0f)
, mHitsRemaining(1)
{

}

// ------------------------------------------------------------- //

Goomba::~Goomba()
{

}

// ------------------------------------------------------------- //

BaseObject* Goomba::Clone(std::string data)
{
	// First extra the data from the line passed in
	std::stringstream dataLine(data);

	Vector2D newPos;
	char     facingDirection;

	dataLine >> newPos.x >> newPos.y >> facingDirection;

	bool startFacingLeft = facingDirection == 'L' ? true : false;

	if (mThisSpriteSheet)
		return new Goomba(newPos, false, mRenderer, mThisSpriteSheet->GetFilePath(), mSpritesOnWidth, mSpritesOnHeight, mCollisionBox.x, mCollisionBox.y, mTimePerFrame, mCanMove, mCanJump, startFacingLeft);
	else
		return nullptr;
}

// ------------------------------------------------------------- //

bool Goomba::Update(const float deltaTime, const Vector2D playerPosition, InteractableLayer* interactionLayer)
{
	if (!mUpdatedStaticVariables)
		UpdateStaticVariables(deltaTime);

	if (mHitsRemaining == 0)
	{
		mTimerTillDespawn -= deltaTime;

		if (mTimerTillDespawn <= 0.0f)
			return true;
	}

	return false;
}

// ------------------------------------------------------------- //

void Goomba::Move()
{

}

// ------------------------------------------------------------- //

void Goomba::Jump()
{

}

// ------------------------------------------------------------- //

void Goomba::Attack()
{

}

// ------------------------------------------------------------- //

void Goomba::UpdateStaticVariables(const float deltaTime)
{
	mTimeRemainingTillNextFrame -= deltaTime;

	if (mTimeRemainingTillNextFrame <= 0.0f)
	{
		mTimeRemainingTillNextFrame = mTimePerFrame;

		mCurrentSpriteID++;

		if (mCurrentSpriteID > mEndSpriteID)
		{
			mCurrentSpriteID = mStartSpriteID;
		}
	}

	mUpdatedStaticVariables = true;
}

// ------------------------------------------------------------- //

void Goomba::Render(const Vector2D renderReferencePoint)
{
	if(mHitsRemaining == 0)
		RenderSprite(renderReferencePoint, 2);
	else
		RenderSprite(renderReferencePoint, mCurrentSpriteID);

}

// ------------------------------------------------------------- //

ObjectCollisionHandleData Goomba::SetIsCollidedWith(TwoDimensionalCollision collisionData)
{
	if(mHitsRemaining == 0)
		return ObjectCollisionHandleData(false, false, false, false);

	if (collisionData.playerPriorPosition.y < mCurrentPosition.y && collisionData.collisionDataPrimary == MOVEMENT_DIRECTION::DOWN)
	{
		// Stop the goomba
		mCanMove  = false;

		if (mHitsRemaining > 0)
		{
			mHitsRemaining--;

			// Set the correct sprite
			//mCurrentSpriteID = 2;
			//mEndSpriteID     = 2;
			//mStartSpriteID   = 2;
		}

		return ObjectCollisionHandleData(false, false, true, false);
	}

	if(collisionData.collisionDataSecondary == MOVEMENT_DIRECTION::RIGHT || collisionData.collisionDataSecondary == MOVEMENT_DIRECTION::LEFT)
		return ObjectCollisionHandleData(false, true, false, false);

	return ObjectCollisionHandleData();
}

// ------------------------------------------------------------- //
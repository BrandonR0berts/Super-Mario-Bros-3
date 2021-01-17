#include "Para_Goomba_SMB3.h"

#include <sstream>

// ------------------------------------------------------------------------------------------------ //

ParaGoomba::ParaGoomba(const Vector2D      spawnPosition,
	const bool          startSpawnedInLevel,
	SDL_Renderer* renderer,
	const std::string   filePathToSpriteSheet,
	const unsigned int  spritesOnWidth,
	const unsigned int  spritesOnHeight,
	const double        collisionBoxWidth,
	const double        collisionBoxHeight,
	const float		   timePerFrame,
	const bool          canMove,
	const bool          canJump,
	const bool          startFacingLeft)

: Goomba(spawnPosition
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
{
	mEndSpriteID   = 0;
	mStartSpriteID = 0;
}

// ------------------------------------------------------------------------------------------------ //

ParaGoomba::~ParaGoomba()
{

}

// ------------------------------------------------------------------------------------------------ //

BaseObject* ParaGoomba::Clone(std::string data)
{
	std::stringstream streamLine(data);

	Vector2D newPos;
	char     facingDirectionChar;
	char     colourChar;

	streamLine >> newPos.x >> newPos.y >> facingDirectionChar >> colourChar;

	bool startMovingLeft = true;
	if (facingDirectionChar == 'R')
		startMovingLeft = false;

	if (mThisSpriteSheet)
		return new ParaGoomba(newPos, false, mRenderer, mThisSpriteSheet->GetFilePath(), mSpritesOnWidth, mSpritesOnHeight, mCollisionBox.x, mCollisionBox.y, mTimePerFrame, mCanJump, mCanJump, startMovingLeft);
	else 
		return nullptr;
}

// ------------------------------------------------------------------------------------------------ //

bool ParaGoomba::Update(const float deltaTime, const Vector2D playerPosition)
{
	PhysicalObject::Update(deltaTime, playerPosition);

	return false;
}

// ------------------------------------------------------------------------------------------------ //

void ParaGoomba::Move()
{

}

// ------------------------------------------------------------------------------------------------ //

void ParaGoomba::Jump()
{

}

// ------------------------------------------------------------------------------------------------ //

void ParaGoomba::Attack()
{

}

// ------------------------------------------------------------------------------------------------ //
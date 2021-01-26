#include "EnemyObject.h"

// ------------------------------------------------------------- //

EnemyObject::EnemyObject(const Vector2D      spawnPosition,
					     const bool          startSpawnedInLevel,
						 SDL_Renderer* renderer,
						 const std::string   filePathToSpriteSheet,
						 const unsigned int  spritesOnWidth,
						 const unsigned int  spritesOnHeight,
						 const double        collisionBoxWidth,
						 const double        collisionBoxHeight,
						 const float		 timePerFrame,
	                     const bool          canMove,
	                     const bool          canJump,
	                     const bool          startFacingLeft) 
: PhysicalObject(spawnPosition
, startSpawnedInLevel
, renderer
, filePathToSpriteSheet
, spritesOnWidth
, spritesOnHeight
, collisionBoxWidth
, collisionBoxHeight
, timePerFrame)

, mVelocity(0, 0)
, mCanJump(canJump)
, mCanMove(canMove)
, mFacingLeft(startFacingLeft)
{

}

// ------------------------------------------------------------- //

EnemyObject::~EnemyObject()
{

}

// ------------------------------------------------------------- //

bool EnemyObject::Update(const float deltaTime, const Vector2D playerPosition, InteractableLayer* interactionLayer)
{
	return true;
}

// ------------------------------------------------------------- //

void EnemyObject::Attack()
{

}

// ------------------------------------------------------------- //

void EnemyObject::Move()
{

}

// ------------------------------------------------------------- //

void EnemyObject::Jump()
{

}

// ------------------------------------------------------------- //
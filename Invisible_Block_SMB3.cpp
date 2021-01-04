#include "Invisible_Block_SMB3.h"

#include "CollectableObject.h"

// ------------------------------------------------------------------------------ //

InvisibleBlock::InvisibleBlock(const Vector2D           spawnPosition,
	const bool				 startSpawnedInLevel,
	SDL_Renderer*            renderer,
	const std::string		 filePathToSpriteSheet,
	const unsigned int		 spritesOnWidth,
	const unsigned int	     spritesOnHeight,
	const double		     collisionBoxWidth,
	const double			 collisionBoxHeight,
	const float				 timePerFrame,
	const unsigned int		 hitsBlockCanTake,
	const POWER_UP_TYPE      powerUpMinimumForDamage,
	const bool               objectReleaseScales,
	const CollectableObject* baseObjectReleased,
	const CollectableObject* maxObjectReleased)

: BlockObject(spawnPosition
, startSpawnedInLevel
, renderer
, filePathToSpriteSheet
, spritesOnWidth
, spritesOnHeight
, collisionBoxWidth
, collisionBoxHeight
, timePerFrame
, hitsBlockCanTake
, powerUpMinimumForDamage
, objectReleaseScales
, baseObjectReleased
, maxObjectReleased)
{

}

// ------------------------------------------------------------------------------ //

InvisibleBlock::~InvisibleBlock()
{

}

// ------------------------------------------------------------------------------ //

BaseObject* InvisibleBlock::Clone(std::string dataLine)
{
	return nullptr;
}

// ------------------------------------------------------------------------------ //

bool InvisibleBlock::Update(const float deltaTime, const Vector2D playerPosition)
{
	return false;
}

// ------------------------------------------------------------------------------ //
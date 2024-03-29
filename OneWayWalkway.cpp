#include "OneWayWalkway.h"

#include <sstream>

// ---------------------------------------------------------------------------- //

OneWayWalkway::OneWayWalkway( const Vector2D      spawnPosition
							, const bool          startSpawnedInLevel
							, SDL_Renderer*       renderer
							, const std::string   filePathToSpriteSheet
							, const unsigned int  spritesOnWidth
							, const unsigned int  spritesOnHeight
							, const double        collisionBoxWidth
							, const double        collsiionBoxHeight
							, const float         timePerFrame) 
	: PhysicalObject(spawnPosition, startSpawnedInLevel, renderer, filePathToSpriteSheet, spritesOnWidth, spritesOnHeight, collisionBoxWidth, collsiionBoxHeight, timePerFrame)
	, mTimePerframe(timePerFrame)
	, mCurrentFrameID(0)
	, mCollisionOffsetY(0.75f)
{

}

// ---------------------------------------------------------------------------- //

OneWayWalkway::~OneWayWalkway()
{

}

// ---------------------------------------------------------------------------- //

BaseObject* OneWayWalkway::Clone(std::string dataLine)
{
	std::stringstream ssLine(dataLine);

	Vector2D position;

	ssLine >> position.x >> position.y;

	return new OneWayWalkway(position, true, mRenderer, "", 1, 1, 1.0f, mCollisionBox.y, 0.0f);
}

// ---------------------------------------------------------------------------- //

RenderData OneWayWalkway::GetRenderData()
{
	return RenderData{mCurrentFrameID, 0, 0, mTimePerframe, 0};
}

// ---------------------------------------------------------------------------- //

ObjectCollisionHandleData OneWayWalkway::SetIsCollidedWith(TwoDimensionalCollision collisionData, const unsigned int playerMovements, const bool isPlayer)
{
	if (!isPlayer)
		return ObjectCollisionHandleData();

	if(collisionData.collisionDataPrimary == MOVEMENT_DIRECTION::DOWN)
		return ObjectCollisionHandleData(false, false, true, false, false, false);

	return ObjectCollisionHandleData(false, false, false, false, false, false);
}

// ---------------------------------------------------------------------------- //
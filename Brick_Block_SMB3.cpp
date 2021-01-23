#include "Brick_Block_SMB3.h"

#include <sstream>

unsigned int BrickBlock::mCurrentSpriteID = 0;
unsigned int BrickBlock::mStartSpriteID   = 0;
unsigned int BrickBlock::mEndSpriteID     = 3;

float        BrickBlock::mTimeRemainingTillNextFrame = 0.0f;

bool         BrickBlock::mUpdatedStaticVariables = false;

// -------------------------------------------------------------------------------------------------- //

BrickBlock::BrickBlock(const Vector2D      spawnPosition,
	const bool          startSpawnedInLevel,
	SDL_Renderer*        renderer,
	const std::string   filePathToSpriteSheet,
	const unsigned int  spritesOnWidth,
	const unsigned int  spritesOnHeight,
	const double        collisionBoxWidth,
	const double        collisionBoxHeight,
	const float		    timePerFrame,
	const unsigned int  hitsBlockCanTake,
	const POWER_UP_TYPE powerUpMinimumForDamage,
	const bool          objectReleaseScales,
	const CollectableObject* baseObjectReleased,
	const CollectableObject* maxObjectReleased,
	const bool               canTurnToCoin)

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
, mTimePerFrame(timePerFrame)
, mCanTurnToCoin(canTurnToCoin)
{
	mHitsBlockCanTake = 1;
}

// -------------------------------------------------------------------------------------------------- //

BrickBlock::~BrickBlock() { ; }

// -------------------------------------------------------------------------------------------------- //

BaseObject* BrickBlock::Clone(std::string dataForNewObject)
{
	// Variables needed for this cloning
	Vector2D            newPos;
	bool                canTurnToCoin;
	char                canTurnToCoinChar;
	std::string         newItemTypeString;
	CollectableObject*  newItemType;

	// Convert the line to something useful
	std::stringstream dataLine(dataForNewObject);

	// Extract the data
	dataLine >> newPos.x >> newPos.y;
	dataLine >> canTurnToCoinChar;
	dataLine >> newItemTypeString;

	if (canTurnToCoinChar == 'T')
		canTurnToCoin = true;
	else 
		canTurnToCoin = false;
	
	newItemType = ConvertFromStringToItemType[newItemTypeString];

	if(mThisSpriteSheet)
	{
	// Return the new instance
	return new BrickBlock(newPos, 
		                  false, 
		                  mRenderer, 
		                  mThisSpriteSheet->GetFilePath(), 
		                  mSpritesOnWidth, 
						  mSpritesOnHeight, 
		                  mCollisionBox.x, 
		                  mCollisionBox.y, 
		                  mTimePerFrame, 
		                  1, 
		                  mPowerUpTypeForDamage, 
		                  false, 
		                  newItemType,
						  nullptr,
		                  mCanTurnToCoin);
	}
	else 
		return nullptr;

}

// -------------------------------------------------------------------------------------------------- //

bool BrickBlock::Update(const float deltaTime, const Vector2D playerPosition)
{
	if(!mUpdatedStaticVariables)
		UpdateStaticVariables(deltaTime);

	return false;
}

// -------------------------------------------------------------------------------------------------- //

void BrickBlock::UpdateStaticVariables(const float deltaTime)
{
	// Update the frame time and adjust the current frame if needed
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

// -------------------------------------------------------------------------------------------------- //

void BrickBlock::Render(const Vector2D renderReferencePoint)
{
	RenderSprite(renderReferencePoint, mCurrentSpriteID);
}

// -------------------------------------------------------------------------------------------------- //
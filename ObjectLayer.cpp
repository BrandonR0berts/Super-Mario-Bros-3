#include "ObjectLayer.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <string.h>

#include "AllObjectsInclude.h"

#include "Constants_SMB3.h"
#include "Animation_Speeds.h"

#include "InteractionLayer.h"
#include "Observer.h"

// -------------------------------------------------------------------------------------------------------------------------- //

ObjectLayer::ObjectLayer(std::string        filePathToDataFile, 
	                     SDL_Renderer*      renderer,
						 InteractableLayer* interactionLayer,
						 std::vector<Observer*> observers)
: mRenderer(renderer)
, mInteractionLayer(interactionLayer)
, mLevelEndObjectCollected(false)
, mCountDownTillReturnToMap(4.0f)
{
	InstantiateNameConversions();

	// Load in the data from the file
	if (!LoadInDataFromFile(filePathToDataFile, observers))
	{
		std::cout << "Failed to load in the objects for this level area: " << filePathToDataFile << std::endl;
		return;
	}

	// Spawn in the initial objects
	CheckIfObjectsShouldSpawn(GetSpawnPoint(0));
}

// -------------------------------------------------------------------------------------------------------------------------- //

ObjectLayer::~ObjectLayer()
{
	if (!mNameToObjectConversion.empty())
	{
		DestroyAllNameConversions();
		mNameToObjectConversion.clear();
	}

	// Clear up all memory used
	for (unsigned int i = 0; i < mUnspawnedObjectsInLevel.size(); i++)
	{
		delete mUnspawnedObjectsInLevel[i];
		mUnspawnedObjectsInLevel[i] = nullptr;
	}
	mUnspawnedObjectsInLevel.clear();

	for (unsigned int i = 0; i < mSpawnedObjectsInLevel.size(); i++)
	{
		delete mSpawnedObjectsInLevel[i];
		mSpawnedObjectsInLevel[i] = nullptr;
	}
	mSpawnedObjectsInLevel.clear();

	// Clear the spawnpoint store
	mSpawnPoints.clear();
}

// -------------------------------------------------------------------------------------------------------------------------- //

void ObjectLayer::Render(const Vector2D gridReferencePoint)
{
	// Loop through all of the spawned objects are render them
	for (unsigned int i = 0; i < mSpawnedObjectsInLevel.size(); i++)
	{
		if(mSpawnedObjectsInLevel[i])
			mSpawnedObjectsInLevel[i]->Render(gridReferencePoint);
	}
}

// -------------------------------------------------------------------------------------------------------------------------- //

bool ObjectLayer::Update(const float deltaTime, SDL_Event e, Vector2D gridReferencePoint)
{
	// Update the currently spawned objects
	UpdateSpawnedObjects(deltaTime, gridReferencePoint);

	// See if any of the object not currently spawned should spawn again
	CheckIfObjectsShouldSpawn(gridReferencePoint);

	// Now check to see if the level has finished
	if (mLevelEndObjectCollected)
	{
		mCountDownTillReturnToMap -= deltaTime;

		if (mCountDownTillReturnToMap <= 0.0f)
			return true;
	}

	return false;
}

// -------------------------------------------------------------------------------------------------------------------------- //

void ObjectLayer::CheckIfObjectsShouldSpawn(const Vector2D gridReferencePoint)
{
	std::vector<unsigned int> removeIDs;

	// Loop through all unspawned objects and check if they need to be spawned
	for (unsigned int i = 0; i < mUnspawnedObjectsInLevel.size(); i++)
	{
		if (mUnspawnedObjectsInLevel[i])
		{
			// If an object is instance locked and is outside of the play area then un-instance lock it
			if (mUnspawnedObjectsInLevel[i]->GetIsInstanceLocked() && !InPlayArea(mUnspawnedObjectsInLevel[i]->GetSpawnPosition(), gridReferencePoint))
			{
				// Unlock it
				mUnspawnedObjectsInLevel[i]->SetInstanceLocked(false);

				// Continue as the check below required the object to be in the play area - to be in here it cannot be
				continue;
			}

			// If the object exists, and is in the play area, and is not instance locked
			if (InPlayArea(mUnspawnedObjectsInLevel[i]->GetSpawnPosition(), gridReferencePoint) && !mUnspawnedObjectsInLevel[i]->GetIsInstanceLocked())
			{
				// Then set this object should spawn into the world
				mSpawnedObjectsInLevel.push_back(mUnspawnedObjectsInLevel[i]);

				mUnspawnedObjectsInLevel[i]->SetPositionToSpawn();

				removeIDs.push_back(i);

				continue;
			}
		}
	}

	unsigned int offset = 0;

	// Now remove these from the other list
	while (removeIDs.size() > 0)
	{
		mUnspawnedObjectsInLevel.erase(mUnspawnedObjectsInLevel.begin() + (removeIDs[0] - offset));
		offset++;

		removeIDs.erase(removeIDs.begin());
	}
}

// -------------------------------------------------------------------------------------------------------------------------- //

bool ObjectLayer::InPlayArea(const Vector2D testPosition, const Vector2D gridReferencePoint)
{
	const double XDistance = abs(gridReferencePoint.x - testPosition.x);

	// First check if the X is valid
	if (XDistance < SCREEN_WIDTH_GRID_SMB3)
	{
		return true;
	}

	// Not in the bounds so return false.
	return false;
}

// -------------------------------------------------------------------------------------------------------------------------- //

void ObjectLayer::UpdateSpawnedObjects(const float deltaTime, Vector2D gridReferencePoint)
{
	// Loop through all spawned objects and check if they need to be unspawned ('destroyed')
	for (unsigned int i = 0; i < mSpawnedObjectsInLevel.size(); i++)
	{
		// Check to see if the object exists
		if (mSpawnedObjectsInLevel[i])
		{
			// If they return true then they need to be unspawned / destroyed
			if (mSpawnedObjectsInLevel[i]->Update(deltaTime, gridReferencePoint, mInteractionLayer, this) || !InPlayArea(mSpawnedObjectsInLevel[i]->GetCurrentPosition(), gridReferencePoint))
			{
				// First store internally that this object has been removed from active play
				mSpawnedObjectsInLevel[i]->SetInstanceLocked(true);

				// Add it to the other list
				mUnspawnedObjectsInLevel.push_back(mSpawnedObjectsInLevel[i]);

				// Remove this object from the spawned list
				mSpawnedObjectsInLevel.erase(mSpawnedObjectsInLevel.begin() + i);
			}
		}
	}

	// Now reset the bool making sure that the animation update only gets called once per frame
	for (unsigned int i = 0; i < mSpawnedObjectsInLevel.size(); i++)
	{
		// Check to see if the object exists
		if (mSpawnedObjectsInLevel[i])
		{
			// Set all spawned objects to have reset their class specific animation values
			mSpawnedObjectsInLevel[i]->ResetUpdatedStaticVariables();
		}
	}
}

// -------------------------------------------------------------------------------------------------------------------------- //

bool ObjectLayer::LoadInDataFromFile(std::string filePath, std::vector<Observer*> observers)
{
	// First we need to open the file and then read in the data in in the correct format
	std::ifstream dataFile(filePath);
	if (!dataFile)
	{
		std::cout << "Failed to open interaction data file: " << filePath << std::endl;
		return false;
	}

	std::string       line, objectNameLine;

	unsigned int      failsafeCounter = 0;

	// Loop through the entire file untill it is empty
	while (std::getline(dataFile, line))
	{
		if (failsafeCounter++ > FAILSAFE_MAX_COUNT_FILE_LOADING_LOOPS)
		{
			dataFile.close();
			break;
		}

		// First check to see if we have hit the end of the file
		if (line == "END")
		{
			dataFile.close();
			return true;
		}
		
		// Check if the line is empty or is a comment
		if (line == "" || line[0] == '#')
			continue;

		// If we have hit 
		if (line == "NEXT")
		{
			objectNameLine = "";
			continue;
		}

		// Now check if we are waiting for a new name of definition
		if (objectNameLine == "")
		{
			objectNameLine = line;
			continue;
		}
		else
		{
			// First check if it is a spawnpoint as they need to be handelled seperately from the other objects
			if (objectNameLine == "SPAWN_POINT")
			{
				mSpawnPoints.push_back(SpawnPoint(line));
				continue;
			}

			// Create a new instance of the object
			mUnspawnedObjectsInLevel.push_back(mNameToObjectConversion[objectNameLine]->Clone(line));

			if (mUnspawnedObjectsInLevel[mUnspawnedObjectsInLevel.size() - 1])
			{
				// Add the audio observer to the object
				for (unsigned int i = 0; i < observers.size(); i++)
				{
					mUnspawnedObjectsInLevel[mUnspawnedObjectsInLevel.size() - 1]->AddObserver(observers[i]);
				}
				// Check if the object should start spawned in the level
				if (mUnspawnedObjectsInLevel[mUnspawnedObjectsInLevel.size() - 1] && mUnspawnedObjectsInLevel[mUnspawnedObjectsInLevel.size() - 1]->GetIsSpawnedInLevel())
				{
					mSpawnedObjectsInLevel.push_back(mUnspawnedObjectsInLevel[mUnspawnedObjectsInLevel.size() - 1]);
					mUnspawnedObjectsInLevel.pop_back();
				}
			}
			else
			{
				mUnspawnedObjectsInLevel.pop_back();
			}
			continue;
		}
	}

	return false;
}

// -------------------------------------------------------------------------------------------------------------------------- //

Vector2D ObjectLayer::GetInitialSpawnPoint() const
{
	// Loop through all spawned objects - as spawnpoints are forced to always be spawned - and get the spawnpoint with ID 0
	for (unsigned int i = 0; i < mSpawnPoints.size(); i++)
	{
		if (mSpawnPoints[i].IsIndex(0))
			return mSpawnPoints[i].GetPosition();
	}

	// If we cant find the spawnpoint then just return nothing
	return Vector2D();
}

// -------------------------------------------------------------------------------------------------------------------------- //

Vector2D ObjectLayer::GetSpawnPoint(unsigned int spawnPointIndex) const
{
	// Loop through all spawned objects - as spawnpoints are forced to always be spawned - and get the spawnpoint with ID 0
	for (unsigned int i = 0; i < mSpawnPoints.size(); i++)
	{
		if (mSpawnPoints[i].IsIndex(spawnPointIndex))
			return mSpawnPoints[i].GetPosition();
	}

	// If we cant find the spawnpoint then just return nothing
	return Vector2D();
}

// -------------------------------------------------------------------------------------------------------------------------- //

void ObjectLayer::InstantiateNameConversions()
{
	// Setup the conversions from a string to a base default data type

	// Collectables
	mNameToObjectConversion["COIN"]                = new Coin_SMB3(Vector2D(), false, mRenderer, "SDL_Mario_Project/Objects/Coin.png", 6, 1, 1.0f, 1.0f, false, COIN_ANIMATION_SPEED);

	// Block objects
	mNameToObjectConversion["BRICK_BLOCK"]         = new BrickBlock(Vector2D(), false, mRenderer, "SDL_Mario_Project/Objects/BrickBlock.png", 4, 1, 1.0f, 1.0f, BRICK_BLOCK_ANIMATION_SPEED, 1, POWER_UP_TYPE::MUSHROOM, false, nullptr, nullptr, true);
	mNameToObjectConversion["INVISIBLE_BLOCK"]     = new InvisibleBlock(Vector2D(), false, mRenderer, "SDL_Mario_Project/Objects/InvisibleBlock.png", 2, 1, 1.0f, 1.0f, 0.0f, 1, POWER_UP_TYPE::NONE, false, (CollectableObject*)mNameToObjectConversion["COIN"], nullptr);
	mNameToObjectConversion["QUESTION_MARK_BLOCK"] = new QuestionMarkBlock(Vector2D(), false, mRenderer, "SDL_Mario_Project/Objects/QuestionMarkBlock.png", 5, 1, 1.0f, 1.0f, QUESTION_MARK_BLOCK_ANIMATION_SPEED, 1, POWER_UP_TYPE::NONE, false, (CollectableObject*)mNameToObjectConversion["COIN"], nullptr);

	mNameToObjectConversion["PIPE"]                = new Pipe(Vector2D(), false, mRenderer, "SDL_Mario_Project/Objects/Pipe.png", 8, 12, 2.0f, 2.0f, 0.0f, 2, 2, false, false, PIPE_TYPE::DEFAULT_GREEN, FACING::UP, -1, -1, -1);

	// Enemy Objects
	mNameToObjectConversion["GOOMBA"]             = new Goomba(Vector2D(), false, mRenderer, "SDL_Mario_Project/Enemies/Goomba/Goomba.png", 6, 3, 1.0f, 1.0f, 0.15f, true, false, true);
	mNameToObjectConversion["PARA_GOOMBA"]        = new ParaGoomba(Vector2D(), false, mRenderer, "SDL_Mario_Project/Enemies/Goomba/Goomba.png", 6, 3, 1.0f, 1.0f, 0.3f, true, true, true);

	mNameToObjectConversion["KOOPA_TROOPER"]      = new KoopaTrooper(Vector2D(), false, mRenderer, "SDL_Mario_Project/Enemies/Koopa Trooper/Koopa.png", 14, 3, 1.0f, 1.0f, KOOPA_WALK_ANIMATION_SPEED, true, false, true, 0);
	mNameToObjectConversion["PARA_KOOPA_TROOPER"] = new KoopaTrooper(Vector2D(), false, mRenderer, "SDL_Mario_Project/Enemies/Koopa Trooper/Koopa.png", 14, 3, 1.0f, 1.0f, KOOPA_WALK_ANIMATION_SPEED, true, true, true, 0);

	mNameToObjectConversion["WALK_WAY"]           = new OneWayWalkway(Vector2D(), false, mRenderer, "", 1, 1, 1.0f, 0.25f, 0.0f);

	mNameToObjectConversion["LEVEL_END"]          = new LevelEndObject(Vector2D(), false, mRenderer, "SDL_Mario_Project/Objects/EndingObject.png", 3, 4, 1.0f, 1.0f, 0.1f, "SDL_Mario_Project/Objects/EndSurroundObject.png");

	mNameToObjectConversion["NOTE_BLOCK"]         = new NoteBlock(Vector2D(), false, mRenderer, "SDL_Mario_Project/Objects/NoteBlock.png", 3, 1, 1.0f, 1.0f, 0.1f, -1, POWER_UP_TYPE::NONE, false, nullptr, nullptr);
}

// -------------------------------------------------------------------------------------------------------------------------- //

void ObjectLayer::DestroyAllNameConversions()
{
	// Collectables
	delete mNameToObjectConversion["COIN"];
	mNameToObjectConversion.erase("COIN");

	delete mNameToObjectConversion["BRICK_BLOCK"];
	mNameToObjectConversion.erase("BRICK_BLOCK");

	delete mNameToObjectConversion["INVISIBLE_BLOCK"];
	mNameToObjectConversion.erase("INVISIBLE_BLOCK");

	delete mNameToObjectConversion["QUESTION_MARK_BLOCK"];
	mNameToObjectConversion.erase("QUESTION_MARK_BLOCK");

	delete mNameToObjectConversion["PIPE"];
	mNameToObjectConversion.erase("PIPE");

	delete mNameToObjectConversion["GOOMBA"];
	mNameToObjectConversion.erase("GOOMBA");

	delete mNameToObjectConversion["PARA_GOOMBA"];
	mNameToObjectConversion.erase("PARA_GOOMBA");

	delete mNameToObjectConversion["KOOPA_TROOPER"];
	mNameToObjectConversion.erase("KOOPA_TROOPER");

	delete mNameToObjectConversion["PARA_KOOPA_TROOPER"];
	mNameToObjectConversion.erase("PARA_KOOPA_TROOPER");

	delete mNameToObjectConversion["WALK_WAY"];
	mNameToObjectConversion.erase("WALK_WAY");

	delete mNameToObjectConversion["LEVEL_END"];
	mNameToObjectConversion.erase("LEVEL_END");

	delete mNameToObjectConversion["NOTE_BLOCK"];
	mNameToObjectConversion.erase("NOTE_BLOCK");
}

// -------------------------------------------------------------------------------------------------------------------------- //

MovementPrevention ObjectLayer::CheckCollision(const Vector2D testPosition, const Vector2D velocity, const Vector2D currentPos, const unsigned int currentMovements, bool isPlayer)
{
	Vector2D                objectBottomLeftPos, objectCollisionBox;
	TwoDimensionalCollision collisionData = TwoDimensionalCollision();

	collisionData.playerPriorPosition     = currentPos;

	// Loop through all objects to see if there has been a collision - Only one collision can occur at once
	for (unsigned int i = 0; i < mSpawnedObjectsInLevel.size(); i++)
	{
		// First check the object exists
		if (mSpawnedObjectsInLevel[i])
		{
			// Now check if it is within the space where the test position is
			objectBottomLeftPos = mSpawnedObjectsInLevel[i]->GetCurrentPosition();
			objectCollisionBox  = mSpawnedObjectsInLevel[i]->GetCollisionBox();

			// X check
			if (testPosition.x < objectBottomLeftPos.x || testPosition.x > objectBottomLeftPos.x + objectCollisionBox.x)
				continue;

			// Y check
			if (testPosition.y < objectBottomLeftPos.y - objectCollisionBox.y || testPosition.y > objectBottomLeftPos.y)
				continue;

			// Then we have a collision - so check which direction that this collision has occured from
			if (velocity.y >= 0.0f)
				collisionData.collisionDataPrimary = MOVEMENT_DIRECTION::DOWN;
			else
				collisionData.collisionDataPrimary = MOVEMENT_DIRECTION::UP;

			if (velocity.x > 0.0f)
				collisionData.collisionDataSecondary = MOVEMENT_DIRECTION::LEFT;
			else if (velocity.x < 0.0f)
				collisionData.collisionDataSecondary = MOVEMENT_DIRECTION::RIGHT;
			else
				collisionData.collisionDataSecondary = MOVEMENT_DIRECTION::NONE;

			// Notify the object that it has been collided with
			ObjectCollisionHandleData objectReturnData = mSpawnedObjectsInLevel[i]->SetIsCollidedWith(collisionData, currentMovements, isPlayer);

			// End of level check
			if (objectReturnData.completedLevel)
			{
				mLevelEndObjectCollected = true;
			}

			// Notify the object that they have a collision upon them - they will return if the object needs to be detroyed or not
			if (objectReturnData.shouldDeleteObject)
			{
				delete mSpawnedObjectsInLevel[i];
				mSpawnedObjectsInLevel[i] = nullptr;
				mSpawnedObjectsInLevel.erase(mSpawnedObjectsInLevel.begin() + i);
				continue;
			}

			if (objectReturnData.shouldDamagePlayer || objectReturnData.movementPrevention.StopXMovement || objectReturnData.movementPrevention.StopYMovement)
			{
				return MovementPrevention(  objectReturnData.movementPrevention.StopXMovement,
						                    objectReturnData.movementPrevention.StopYMovement,
						                    objectReturnData.givesJumpLeway,
					                        objectReturnData.shouldDamagePlayer);
			}
		}
	}

	// Return that there has been no collision
	return MovementPrevention(false, false, false, false);
}

// -------------------------------------------------------------------------------------------------------------------------- //
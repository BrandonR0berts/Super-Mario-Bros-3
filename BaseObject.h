#ifndef _BASE_OBJECT_H_
#define _BASE_OBJECT_H_

#include "Game_Maths.h"
#include "Commons.h"
#include "Commons_SMB3.h"

#include <iostream>
#include <string.h>

#include "Subject.h"

class InteractableLayer;
class ObjectLayer;

class BaseObject abstract : public Subject
{
public:
	BaseObject() = delete;
	BaseObject(const Vector2D spawnPosition, const bool startSpawnedInLevel);
	virtual ~BaseObject();

	virtual BaseObject* Clone(std::string dataForNewObject) = 0;

	virtual void Render(const Vector2D renderReferencePoint);
	virtual bool Update(const float deltaTime, const Vector2D playerPosition, InteractableLayer* interactionLayer, ObjectLayer* objectLayer);

	bool         GetIsSpawnedInLevel() const            { return mIsCurrentlySpawnedInLevel; }
	void         SetIsSpawnedInLevel(const bool newVal) { mIsCurrentlySpawnedInLevel = newVal; }

	Vector2D     GetSpawnPosition()               const { return mSpawnPosition; }
	virtual Vector2D     GetCurrentPosition()             const { return mCurrentPosition; }
	
	void         SetInstanceLocked(const bool newVal)   { mIsInstanceLocked = newVal; }
	bool         GetIsInstanceLocked()            const { return mIsInstanceLocked; }

	void         SetPositionToSpawn()                   { mCurrentPosition = mSpawnPosition; }

	virtual void     ResetUpdatedStaticVariables() { ; }
	virtual const Vector2D GetCollisionBox() const { return Vector2D(); }

	virtual ObjectCollisionHandleData SetIsCollidedWith(TwoDimensionalCollision collisionData, const unsigned int playerCurrentMovements, const bool isPlayer) { return ObjectCollisionHandleData(); }

protected:
	Vector2D            mCurrentPosition;
	Vector2D	        mSpawnPosition;

	bool                mIsCurrentlySpawnedInLevel;
	bool                mIsInstanceLocked;
};

#endif // !_BASE_OBJECT_SMB3_H_

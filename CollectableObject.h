#ifndef _COLLECTABLE_OBJECT_BASE_H_
#define _COLLECTABLE_OBJECT_BASE_H_

#include "PhysicalObject.h"

class CollectableObject abstract : public PhysicalObject
{
public:
	CollectableObject() = delete;
	CollectableObject(const Vector2D       spawnPosition,
		              const bool           startSpawnedInLevel,
		              SDL_Renderer*        renderer,
					  const std::string    filePathToSpriteSheet,
					  const unsigned int   spritesOnWidth,
					  const unsigned int   spritesOnHeight,
				      const double         collisionBoxWidth,
					  const double         collisionBoxHeight,
					  const float		   timePerFrame,
		              const bool           collectableCanMove);
	virtual ~CollectableObject() override;

	virtual BaseObject* Clone(std::string dataForNewObject) = 0;

	virtual bool Update(const float deltaTime, const Vector2D playerPosition, InteractableLayer* interactionLayer, ObjectLayer* objectLayer) override;

	virtual void Move();
	virtual void Jump();

	ObjectCollisionHandleData SetIsCollidedWith(TwoDimensionalCollision collisionData, const unsigned int playerMovements, const bool isPlayer) override { return ObjectCollisionHandleData(true, false, false, false, false, false); }

	virtual bool UpdateReleaseAnimation(const float deltaTime, const Vector2D& startPos) { return false; }

protected:
	Vector2D    mVelocity; // velocity for the movement of the collectable

	float       mReleaseAnimationTimer;

	unsigned int mReleaseAnimationStartFrame;
	unsigned int mReleaseAnimationEndFrame;
	unsigned int mReleaseAnimationCurrentFrame;

	Vector2D	 mAnimationStartPosition;
	bool         mTraveledUpOnAnimation;

	bool        mCanMove;
	bool        mCanJump;
	bool        mFacingLeft;
};

#endif
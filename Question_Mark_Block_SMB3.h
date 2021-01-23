#ifndef _QUESTION_MARK_BLOCK_H_
#define _QUESTION_MARK_BLOCK_H_

#include "BlockObject.h"

class QuestionMarkBlock final : public BlockObject
{
public:
	QuestionMarkBlock(const Vector2D           spawnPosition,
					  const bool			   startSpawnedInLevel,
					  SDL_Renderer*			   renderer,
					  const std::string		   filePathToSpriteSheet,
					  const unsigned int	   spritesOnWidth,
					  const unsigned int	   spritesOnHeight,
					  const double			   collisionBoxWidth,
					  const double			   collisionBoxHeight,
					  const float			   timePerFrame,
					  const unsigned int	   hitsBlockCanTake,
					  const POWER_UP_TYPE	   powerUpMinimumForDamage,
					  const bool               objectReleaseScales,
					  const CollectableObject* baseObjectReleased,
					  const CollectableObject* maxObjectReleased);
	~QuestionMarkBlock() override;

	bool Update(const float deltaTime, const Vector2D playerPosition) override;
	void Render(const Vector2D renderReferencePoint) override;

	BaseObject* Clone(std::string dataForNewObject) override;

	void        ResetUpdatedStaticVariables() override { mUpdatedStaticVariables = false; }

private:
	void        UpdateStaticVariables(const float deltaTime);

	RenderData  GetRenderData() override { return RenderData{ mCurrentSpriteID, mStartSpriteID, mEndSpriteID, mTimeRemainingTillNextFrame, mTimePerFrame }; }

	static bool                 mUpdatedStaticVariables;

	static unsigned int         mCurrentSpriteID;
	static unsigned int         mEndSpriteID;
	static unsigned int         mStartSpriteID;

	static float                mTimeRemainingTillNextFrame;
	const float				    mTimePerFrame;
};

#endif
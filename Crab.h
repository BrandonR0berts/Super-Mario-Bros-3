#ifndef _CRAB_SMB1_H_
#define _CRAB_SMB1_H_

#include "RenderObject.h"

struct SDL_Renderer;
class LevelMap;

class Crab final : public RenderObject
{
public:
	Crab(SDL_Renderer* renderer, const char* filePathToSpriteSheet, const float timePerFrame, Vector2D startPos, Vector2D collisionBox, bool spawningOnLeftSide, const float movementSpeed, unsigned int colourID);
	~Crab();

	void SetPOWHit() override;

private:
	bool ClassSpecificUpdate(const float deltaTime) override;

	Texture2D* GetSpriteSheet() override { return mSpriteSheet; }

	static Texture2D*   mSpriteSheet;
	static unsigned int mCrabCount;

	float               mFlipCountdown;
	unsigned int        mSpriteSheetStartPoint;

	bool                mAngry;
};

#endif
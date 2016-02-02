#pragma once
#include "Sprite.h"

class CSpriteSet :	public CSprite
{
protected:
	int setsCnt;
	int spritesInSetsCnt;

	int currSet;
	int currSpriteInSet;

	const CTexCoord* currSpriteInSetCoords;
	bool needToUpdateTexCoords;

	virtual void initSprite(const int atlasID, const int texID);
public:
	const int  getCurrSet()const;
	void setCurrSet(const int set);
	const int  getSpriteInSet()const;
	void setSpriteInSet(const int currSpriteInSet);
	const int  getSetsCnt()const;
	const int  getSpritesInSetsCnt()const;
	const bool isNeedToUpdateTexCoords()const;

	void changeSetParams(const int setsCnt, const int spritesInSetCnt);
	const CTexCoord* getCurrSpriteCoords()const;
		
	virtual void updateTexCoord(const float dt);
	virtual void update(const float dt);
	virtual void updateManuallyVBO();
	
	CSpriteSet(	const int texAtlasID, const int texID, const int spriteW, const int spriteH,
				const int setsCnt, const int spritesInSetsCnt, const int spriteID = Defines::SPRITE_QUAD);
	virtual ~CSpriteSet();
};


#pragma once
#include "SpriteSet.h"

class CAnimatedSprite :	public CSpriteSet
{
protected:
	float timeOfAnimation;
	float currAnimTime;
	float animSpeed;

	bool isStoped;
	bool isLooped;
public:
	const float getTimeOfAnimation()const;
	void  setTimeOfAnimation(const float time);
	const float getCurrAnimTime()const;
	void  setCurrAnimTime(const float time);
	const float getAnimSpeed()const;
	void  setAnimSpeed(const float speed);

	void setIsStoped(const bool isStoped);
	void setIsLooped(const bool isLooped);
	const bool getIsStoped()const;
	const bool getIsLooped()const;

	void resetAnimation();
	void stopAnimation();
	void pauseAnimation();
	void playAnimation();

	virtual void update(const float dt);

	CAnimatedSprite(const int texAtlasID, const int texID, const int spriteW, const int spriteH,
					const int setsCnt, const int spritesInSetsCnt, const int spriteID = Defines::SPRITE_QUAD);
	virtual ~CAnimatedSprite();
};


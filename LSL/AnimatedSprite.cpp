#include "AnimatedSprite.h"

#include <iostream>

CAnimatedSprite::CAnimatedSprite(const int texAtlasID, const int texID, const int spriteW, const int spriteH, const int setsCnt, const int spritesInSetsCnt,
	const int spriteID) : CSpriteSet(texAtlasID, texID, spriteW, spriteH, setsCnt, spritesInSetsCnt, spriteID)
{
	timeOfAnimation = 0.5f;
	currAnimTime = 0.0f;
	animSpeed = 1.0f;

	isStoped = true;
	isLooped = true;

	setType(CGameObj::TYPE_SPRITE_ANIMATED);
}

CAnimatedSprite::~CAnimatedSprite()
{

}

const float CAnimatedSprite::getTimeOfAnimation()const
{
	return timeOfAnimation;
}
void  CAnimatedSprite::setTimeOfAnimation(const float time)
{
	if (time > 0.0f) timeOfAnimation = time;
}
const float CAnimatedSprite::getCurrAnimTime()const
{
	return currAnimTime;
}
void  CAnimatedSprite::setCurrAnimTime(const float time)
{
	if (time > 0.0f) currAnimTime = time;
}
const float CAnimatedSprite::getAnimSpeed()const
{
	return animSpeed;
}
void  CAnimatedSprite::setAnimSpeed(const float speed)
{
	animSpeed = speed;
}

void CAnimatedSprite::setIsStoped(const bool isStoped)
{
	this->isStoped = isStoped;
}
void CAnimatedSprite::setIsLooped(const bool isLooped)
{
	this->isLooped = isLooped;
}
const bool CAnimatedSprite::getIsStoped()const
{
	return isStoped;
}
const bool CAnimatedSprite::getIsLooped()const
{
	return isLooped;
}

void CAnimatedSprite::resetAnimation()
{
	currAnimTime = 0.0f;
		
	setCurrSet(0);
	setSpriteInSet(0);
	isStoped = true;
}
void CAnimatedSprite::stopAnimation()
{
	currAnimTime = 0.0f;
	isStoped = true;
}
void CAnimatedSprite::pauseAnimation()
{
	isStoped = true;
}
void CAnimatedSprite::playAnimation()
{
	isStoped = false;
}

void CAnimatedSprite::update(const float dt)
{
	__super::update(dt);

	if (!isStoped && onScreen)
	{
		if (isLooped && (currAnimTime >= timeOfAnimation))
		{
			currAnimTime = 0.0f;
			setSpriteInSet(0);
		}
		if (currAnimTime >= timeOfAnimation)
		{
			currAnimTime = timeOfAnimation;
			isStoped = true;
		}

		int currSpriteInSet = getSpriteInSet();
		int currSpriteInSetCnt = getSpritesInSetsCnt();

		currAnimTime += dt * animSpeed;		
		if (currAnimTime >= timeOfAnimation * ((float)(currSpriteInSet + 1) / (float)currSpriteInSetCnt))
		{
			setSpriteInSet(currSpriteInSet + 1);
		}
	}
}
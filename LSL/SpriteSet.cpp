#include "SpriteSet.h"
#include "SceneGraphNode.h"
#include "global.h"
#include "VertexBuffer.h"
#include "TextureAtlasManager.h"
#include "TextureAtlas.h"
#include "GLUtils.h"
#include "VideoManeger.h"
#include "Log.h"

CSpriteSet::CSpriteSet(const int texAtlasID, const int texID, const int spriteW, const int spriteH, const int setsCnt, const int spritesInSetsCnt,
	const int spriteID) : CSprite(texAtlasID, texID, spriteW, spriteH, spriteID)
{
	this->setsCnt = setsCnt;
	this->spritesInSetsCnt = spritesInSetsCnt;

	this->currSet = 0;
	this->currSpriteInSet = 0;
	currSpriteInSetCoords = NULL;
	needToUpdateTexCoords = false;

	setType(CGameObj::TYPE_SPRITE_SET);
}

CSpriteSet::~CSpriteSet()
{

}

const bool CSpriteSet::isNeedToUpdateTexCoords()const
{
	return needToUpdateTexCoords;
}

const int  CSpriteSet::getCurrSet()const
{
	return currSet;
}

void CSpriteSet::setCurrSet(const int set)
{
	if (set >= 0 && set < setsCnt)
	{
		this->currSet = set;
		needToUpdateTexCoords = true;
	}
}

const int  CSpriteSet::getSpriteInSet()const
{
	return currSpriteInSet;
}

void CSpriteSet::setSpriteInSet(const int spriteInSet)
{
	if (spriteInSet >= 0 && spriteInSet < spritesInSetsCnt)
	{
		this->currSpriteInSet = spriteInSet;
		needToUpdateTexCoords = true;
	}
}

const int  CSpriteSet::getSetsCnt()const
{
	return setsCnt;
}

const int  CSpriteSet::getSpritesInSetsCnt()const
{
	return spritesInSetsCnt;
}

void CSpriteSet::changeSetParams(const int setsCnt, const int spritesInSetCnt)
{
	this->setsCnt = setsCnt;
	this->spritesInSetsCnt = spritesInSetCnt;

	if (this->setsCnt <= 0) this->setsCnt = 1;
	if (this->spritesInSetsCnt <= 0) this->spritesInSetsCnt = 1;
}

const CTexCoord* CSpriteSet::getCurrSpriteCoords()const
{
	return currSpriteInSetCoords;
}

void CSpriteSet::updateTexCoord(const float dt)
{
	if (!currTexCoords) return;
	currSpriteInSetCoords = currTexCoords->getSprite(currSet, currSpriteInSet);
	if (!currSpriteInSetCoords) return;

	if (needToUpdateVBO)
	{
		CVertexBuffer* VBO = getVBOForModify();
		if (!VBO)
		{
			CLog::getInstance()->addError("CSpriteSet::updateTexCoord() \t| VBO == NULL");
			return;
		}
		CGLUtils::fillVBOTexData(VBO->getTextDataForModify(), NULL, currSpriteInSetCoords, NULL, 0, spriteDirection, 1);
	}
	needToUpdateTexCoords = false;
}

void CSpriteSet::update(const float dt)
{
	__super::update(dt);
	
	if (needToUpdateTexCoords)
	{
		updateTexCoord(dt);		
		updated = true;
	}
}

void CSpriteSet::updateManuallyVBO()
{
	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO)
	{
		CLog::getInstance()->addError("CSpriteSet::updateManuallyVBO() \t| VBO == NULL");
		return;
	}
	CGLUtils::fillVBOVertData(VBO->getVertDataForModify(), 0, this, 1);

	if (!currTexCoords) return;
	currSpriteInSetCoords = currTexCoords->getSprite(currSet, currSpriteInSet);
	if (!currSpriteInSetCoords) return;
	CGLUtils::fillVBOTexData(VBO->getTextDataForModify(), NULL, currSpriteInSetCoords, NULL, 0, spriteDirection, 1);
}

void CSpriteSet::initSprite(const int atlasID, const int texID)
{
	GLfloat vert[] = { 0, 0, 0, CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight, 0 };
	GLfloat tex[] = { 0, 1, 1, 1, 1, 0, 0, 0 };
	short ind[] = { 0, 1, 2, 0, 2, 3 };

	CTextureAtlas* currAtlas = CTextureAtlasManager::getAtlas(atlasID);
	if (!currAtlas) 
	{
		CLog::getInstance()->addError("CSpriteSet::initSprite() \t| currAtlas == NULL");
		return;
	}

	currTexCoords = currAtlas->getTexCoord(texID);
	if (!currTexCoords)
	{
		CLog::getInstance()->addError("CSpriteSet::initSprite() \t| currTexCoords == NULL");
		return;
	}
	if (!currTexCoords->isSet()) 
	{
		CLog::getInstance()->addError("CSpriteSet::initSprite() \t| currTexCoords == NULL");
		return;
	}

	currSpriteInSetCoords = currTexCoords->getSprite(currSet, currSpriteInSet);
	if (!currSpriteInSetCoords) 
	{
		CLog::getInstance()->addError("CSpriteSet::initSprite() \t| currSpriteInSetCoords == NULL");
		return;
	}

	CGLUtils::fillVBOData(vert, tex, NULL, ind, currSpriteInSetCoords, NULL, 0, this, spriteDirection, 1);
	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO)
	{
		CLog::getInstance()->addError("CSpriteSet::initSprite() \t| VBO == NULL");
		return;
	}

	this->atlasID = atlasID;
	this->texID = texID;

	VBO->setVerticesData(vert, 4 * 3);
	VBO->setTexData(tex, 4 * 2);
	VBO->setIndicesData(ind, 6);
	
	needToUpdateTexCoords = false;
	needToUpdateCoords	  = false;
}
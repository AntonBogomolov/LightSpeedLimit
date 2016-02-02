#include "Sprite.h"
#include "SceneGraphNode.h"
#include "global.h"
#include "VertexBuffer.h"
#include "TextureAtlasManager.h"
#include "TextureAtlas.h"
#include "GLUtils.h"
#include "VideoManeger.h"
#include "Camera.h"
#include "Log.h"

CSprite::CSprite(const int texAtlasID, const int texID, const int spriteW, const int spriteH, const int spriteID) : CSceneGraphNodeDrawable()
{
	this->spriteID = spriteID;
	this->spriteDirection = Defines::DIR_UP;

	setWidth(spriteW);
	setHeight(spriteH);

	this->texID   = texID;
	this->atlasID = texAtlasID;
	
	currTexCoords = NULL;
	needToUpdateCoords = false;	
	updated = true;
	needToUpdateVBO = true;
	setType(CGameObj::TYPE_SPRITE);
	isInited = false;
}

CSprite::~CSprite()
{

}

const int CSprite::getSpriteTextureID()const
{
	return texID;
}

const int CSprite::getSpriteAtlasID()const
{
	return atlasID;
}

void CSprite::initSprite(const int atlasID, const int texID)
{
	GLfloat vert[]	= { 0, 0, 0, CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight, 0 };
	GLfloat tex[]	= { 0, 1, 1, 1, 1, 0, 0, 0 };
	short ind[]		= { 0, 1, 2, 0, 2, 3 };

	CTextureAtlas* currAtlas = CTextureAtlasManager::getAtlas(atlasID);
	if (!currAtlas)
	{
		CLog::getInstance()->addError("CSprite::initSprite() \t| currAtlas == NULL");
		return;
	}
	currTexCoords = currAtlas->getTexCoord(texID);
	if (!currTexCoords)
	{
		CLog::getInstance()->addError("CSprite::initSprite() \t| currTexCoords == NULL");
		return;
	}

	CGLUtils::fillVBOData(vert, tex, NULL, ind, currTexCoords, NULL, 0, this, spriteDirection, 1);
	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO)		
	{
		CLog::getInstance()->addError("CSprite::initSprite() \t| VBO == NULL");
		return;
	}

	this->atlasID = atlasID;
	this->texID = texID;

	VBO->setVerticesData(vert, 4 * 3);
	VBO->setTexData(tex, 4 * 2);
	VBO->setIndicesData(ind, 6);

	isInited = true;
}

const bool CSprite::isNeedToUpdateCoords()const
{
	return needToUpdateCoords;
}

const bool CSprite::isUpdated()const
{
	return updated;
}

const bool CSprite::isNeedToUpdateVBO()const
{
	return needToUpdateVBO;
}

void CSprite::isNeedToUpdateVBO(const bool need)
{
	needToUpdateVBO = need;
}

void CSprite::setWidth(const int spriteW)
{
	__super::setWidth(spriteW);
	needToUpdateCoords = true;
}

void CSprite::setHeight(const int spriteH)
{
	__super::setHeight(spriteH);
	needToUpdateCoords = true;
}

const int CSprite::getSpriteDirection()const
{
	return spriteDirection;
}

void CSprite::setSpriteDirection(const int newDir)
{
	spriteDirection = newDir;
}

const int CSprite::getSpriteId()const
{
	return spriteID;
}

void CSprite::setSpriteId(const int spriteId)
{
	this->spriteID = spriteId;
}

const CTexCoord* CSprite::getTexCoords()const
{
	return currTexCoords;
}

void CSprite::updateCoords(const float dt)
{
	if (!onScreen)	return;
	if (needToUpdateVBO)
	{	
		CVertexBuffer* VBO = getVBOForModify();
		if (!VBO)
		{
			CLog::getInstance()->addError("CSprite::updateCoords() \t| VBO == NULL");
			return;
		}
		CGLUtils::fillVBOVertData(VBO->getVertDataForModify(), 0, this, 1);
//		VBO->setVertices(GL_DYNAMIC_DRAW);
	}
	needToUpdateCoords = false;
}

void CSprite::updateManuallyVBO()
{
	if (!isInited)
	{
		initSprite(atlasID, texID);
	}

	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO)
	{
		CLog::getInstance()->addError("CSprite::updateManuallyVBO() \t| VBO == NULL");
		return;
	}
	CGLUtils::fillVBOVertData(VBO->getVertDataForModify(), 0, this, 1);
//	VBO->setVertices(GL_DYNAMIC_DRAW);
}


void CSprite::update(const float dt)
{
	if (!isInited)
	{
		initSprite(atlasID, texID);		
	}

	if (updated) updated = false;		
	if (needToUpdateCoords)
	{
		updateCoords(dt);		
		updated = true;
	}	
}

const std::string CSprite::getSpriteName()const
{	
	return __super::getNodeName();
}

void CSprite::setSpriteName(const std::string name)
{
	__super::setNodeName(name);
}

void CSprite::setCenterAbsoluteCoords(const glm::vec2* newCoords)
{
	__super::setCenterAbsoluteCoords(newCoords);
	needToUpdateCoords = true;
}
void CSprite::setCenterScreenCoords(const glm::vec2* newCoords)
{
	__super::setCenterScreenCoords(newCoords);
	needToUpdateCoords = true;
}
void CSprite::setCenterAbsoluteCoords(const int x, const int y)
{
	__super::setCenterAbsoluteCoords(x, y);
	needToUpdateCoords = true;
}
void CSprite::setCenterScreenCoords(const int x, const int y)
{
	__super::setCenterScreenCoords(x, y);
	needToUpdateCoords = true;
}

void CSprite::setRotAngle(const float angle)
{
	__super::setRotAngle(angle);
	needToUpdateCoords = true;
}
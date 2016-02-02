#pragma once
#include "GameObj.h"
#include "EngineSettings.h"
#include "EngineDefines.h"
#include "SceneGraphNode.h"

#include <string>

class CTexCoord;

class CSprite :	public CSceneGraphNodeDrawable
{
protected:
	int spriteID;
	int spriteDirection;

	int texID;
	int atlasID;

	const CTexCoord* currTexCoords;
	bool needToUpdateCoords;
	bool updated;
	bool needToUpdateVBO;
	bool isInited;

	virtual void initSprite(const int atlasID, const int texID);
public:
	const int  getSpriteTextureID()const;
	const int  getSpriteAtlasID()const;
	const int  getSpriteDirection()const;
	void setSpriteDirection(const int newDir);
	const int  getSpriteId()const;
	void setSpriteId(const int spriteId);
	const CTexCoord* getTexCoords()const;
	
	const bool isNeedToUpdateCoords()const;
	const bool isNeedToUpdateVBO()const;
	void isNeedToUpdateVBO(const bool need);
	const bool isUpdated()const;
	
	virtual void setWidth(const int w);
	virtual void setHeight(const int h);
	virtual void setCenterAbsoluteCoords(const glm::vec2* newCoords);
	virtual void setCenterScreenCoords(const glm::vec2* newCoords);
	virtual void setCenterAbsoluteCoords(const int x, const int y);
	virtual void setCenterScreenCoords(const int x, const int y);
	virtual void setRotAngle(const float angle);
	
	const std::string getSpriteName()const;
	void setSpriteName(const std::string name);

	virtual void updateCoords(const float dt);
	virtual void update(const float dt);
	virtual void updateManuallyVBO();
public:
	CSprite(const int texAtlasID, const int texID, const int spriteW, const int spriteH, const int spriteID = Defines::SPRITE_QUAD);
	virtual ~CSprite();
};
#pragma once
#include "SceneGraphNode.h"
#include "GameObj.h"

#include <vector>

class CSprite;

class CSpriteCollection : public CSceneGraphNodeDrawable
{
protected:
	std::vector<CSprite*>* sprites;

	bool needToRebuildVBO;
public:
	const bool isNeedToRebuildVBO()const;
	void setNeedToRebuildVBO(const bool need);

	void addSpriteToCollection(CSprite* sprite);
	void clearSpriteCollection();
	void deleteSpriteFromCollection(const CSprite* sprite);
	const int  getSpritesCntInCollection()const;
	const std::vector<CSprite*>* getSprites()const;

	void rebuidVBO();
	void rebuidVBOCoords();
	void rebuidVBOTexCoords();

	virtual void updateCoords(const float dt);
	virtual void update(const float dt);

	CSpriteCollection(const int type = TYPE_SPRITE_COLLECTION);
	virtual ~CSpriteCollection();
};


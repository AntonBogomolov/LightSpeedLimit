#include "SpriteCollection.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "SceneGraphNode.h"
#include "global.h"
#include "VertexBuffer.h"
#include "TextureAtlasManager.h"
#include "TextureAtlas.h"
#include "Camera.h"
#include "GLUtils.h"
#include "Log.h"

CSpriteCollection::CSpriteCollection(int type) : CSceneGraphNodeDrawable()
{
	sprites = new std::vector<CSprite*>();
	needToRebuildVBO = true;

	setType(CGameObj::TYPE_SPRITE_COLLECTION);
}

CSpriteCollection::~CSpriteCollection()
{
	if (sprites)
	{
		sprites->clear();
		delete sprites;
	}
}


const bool CSpriteCollection::isNeedToRebuildVBO()const
{
	return needToRebuildVBO;
}

void CSpriteCollection::setNeedToRebuildVBO(const bool need)
{
	needToRebuildVBO = need;
}

void CSpriteCollection::addSpriteToCollection(CSprite* sprite)
{
	if (!sprite)
	{
		CLog::getInstance()->addWarning("CSpriteCollection::addSpriteToCollection() \t| sprite == NULL");
		return;
	}

	sprite->isNeedToUpdateVBO(false);
	sprites->push_back(sprite);
}

void CSpriteCollection::clearSpriteCollection()
{
	sprites->clear();
}

void CSpriteCollection::deleteSpriteFromCollection(const CSprite* sprite)
{
	if (!sprite) 	
	{
		CLog::getInstance()->addWarning("CSpriteCollection::deleteSpriteFromCollection() \t| sprite == NULL");
		return;
	}
	for (auto it = sprites->begin(); it != sprites->end(); ++it)
	{
		if (sprite == (*it))
		{
			sprites->erase(it);
			break;
		}
	}	
}

const int CSpriteCollection::getSpritesCntInCollection()const
{
	return sprites->size();
}

const std::vector<CSprite*>* CSpriteCollection::getSprites()const
{
	return sprites;
}

void CSpriteCollection::rebuidVBO()
{
	CSprite* currSprite;
	CSpriteSet* currSpriteSet;
	const CTexCoord* texCoord;

	GLfloat*	vert = new GLfloat[sprites->size() * 12];
	GLfloat*	tex  = new GLfloat[sprites->size() * 8];
	short*		ind  = new short[sprites->size() * 6];

	int index = 0;
	for (auto it = sprites->begin(); it != sprites->end(); ++it)
	{
		currSprite = (*it);
		if (currSprite->getType() == TYPE_SPRITE) currSpriteSet = NULL;
		else currSpriteSet = (CSpriteSet*)currSprite;

		texCoord = currSprite->getTexCoords();
		if (currSpriteSet)
		{
			if (texCoord) texCoord = currSpriteSet->getCurrSpriteCoords();
		}
		
		if (!texCoord) continue;
		CGLUtils::fillVBOData(vert, tex, NULL, ind, texCoord, NULL, index, currSprite, currSprite->getSpriteDirection(), 1);
		
		index++;
	}

	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO) 
	{
		CLog::getInstance()->addError("CSpriteCollection::rebuidVBO() \t| VBO == NULL");
		return;
	}

	VBO->setVerticesData(vert, 4 * 3 * index);
	VBO->setTexData(tex, 4 * 2 * index);
	VBO->setIndicesData(ind, 6 * index);

	delete [] vert;
	delete [] tex;
	delete [] ind;

	needToRebuildVBO = false;
}

void CSpriteCollection::rebuidVBOCoords()
{
	CSprite* currSprite;
	GLfloat*	vert = new GLfloat[sprites->size() * 12];
	
	int index = 0;
	for (auto it = sprites->begin(); it != sprites->end(); ++it)
	{
		currSprite = (*it);				
		CGLUtils::fillVBOVertData(vert, index, currSprite, 1);
		index++;
	}

	CVertexBuffer* VBO = getVBOForModify();	
	if (!VBO)
	{
		CLog::getInstance()->addError("CSpriteCollection::rebuidVBOCoords() \t| VBO == NULL");
		return;
	}

	VBO->setVerticesData(vert, 4 * 3 * index);
	delete[] vert;	
}

void CSpriteCollection::rebuidVBOTexCoords()
{
	CSprite* currSprite;
	CSpriteSet* currSpriteSet;
	const CTexCoord* texCoord;
		
	GLfloat* tex = new GLfloat[sprites->size() * 8];
	
	int index = 0;
	for (auto it = sprites->begin(); it != sprites->end(); ++it)
	{
		currSprite = (*it);
		if (currSprite->getType() == TYPE_SPRITE) currSpriteSet = NULL;
		else currSpriteSet = (CSpriteSet*)currSprite;

		texCoord = currSprite->getTexCoords();
		if (currSpriteSet)
		{
			if (texCoord) texCoord = currSpriteSet->getCurrSpriteCoords();
		}

		if (!texCoord) continue;
		CGLUtils::fillVBOTexData( tex, NULL, texCoord, NULL, index, currSprite->getSpriteDirection(), 1);

		index++;
	}

	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO)
	{
		CLog::getInstance()->addError("CSpriteCollection::rebuidVBOTexCoords() \t| VBO == NULL");
		return;
	}
			
	VBO->setTexData(tex, 4 * 2 * index);
	delete[] tex;
}

void CSpriteCollection::updateCoords(const float dt)
{

}

void CSpriteCollection::update(const float dt)
{		
	if (needToRebuildVBO)
	{
		rebuidVBO();
	}
	for (auto it = sprites->begin(); it != sprites->end(); ++it)
	{
		CSprite* currSprite = (*it);		
		currSprite->update(dt);
		if (currSprite->isUpdated()) needToRebuildVBO = true;	
	}	
}
#include "TextureAtlasManager.h"
#include "TextureAtlas.h"


CTextureAtlasManager::~CTextureAtlasManager()
{
	cleanup();	
}

int						CTextureAtlasManager::atlasCount = 10;
CTextureAtlasManager*	CTextureAtlasManager::instance = NULL;
std::map<int, CTextureAtlas*> CTextureAtlasManager::textureAtlas;

CTextureAtlasManager::CTextureAtlasManager(int atlCnt)
{
	atlasCount = atlCnt;
	textureAtlas.clear();
	instance = NULL;
}


CTextureAtlasManager* CTextureAtlasManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new CTextureAtlasManager();
	}
	return instance;
}

void CTextureAtlasManager::cleanup()
{	
	for each (auto currPair in textureAtlas)
	{
		CTextureAtlas* currAtl = currPair.second;
		if (currAtl != NULL)
		{
			currAtl->cleanup();
			delete currAtl;
			currAtl = NULL;
		}
	}
	textureAtlas.clear();
}

CTextureAtlas* CTextureAtlasManager::getAtlas(int atlID)
{	
	return textureAtlas[atlID];
}

CTextureAtlas* CTextureAtlasManager::addAtlas(int atlID)
{
	if (textureAtlas[atlID] == NULL)
	{
		textureAtlas[atlID] = new CTextureAtlas();
		return textureAtlas[atlID];
	}

	textureAtlas[atlID]->setIsUsing(true);
	return textureAtlas[atlID];
}

void CTextureAtlasManager::delAtlas(int atlID)
{
	if (textureAtlas[atlID] != NULL)
	{
		textureAtlas[atlID]->cleanup();
		delete textureAtlas[atlID];
	}
	textureAtlas[atlID] = NULL;
	textureAtlas.erase(atlID);
}

void CTextureAtlasManager::lazyCleanup()
{
	for each (auto currPair in textureAtlas)
	{
		currPair.second->setIsUsing(false);
	}
}

void CTextureAtlasManager::cleanupUnused()
{
	for each (auto currPair in textureAtlas)
	{
		CTextureAtlas* currAtl = currPair.second;
		if (currAtl != NULL)
		{
			if (currAtl->getIsUsing()) continue;

			currAtl->cleanup();
			delete currAtl;
			currAtl = NULL;
		}
	}
}

void CTextureAtlasManager::generateAllAtlases()
{
	for each (auto currPair in textureAtlas)
	{
		currPair.second->generateAtlas();
	}
}
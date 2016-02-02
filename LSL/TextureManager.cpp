#include "TextureManager.h"
#include "Texture.h"

int					CTextureManager::texturesCount = 200;
CTextureManager*	CTextureManager::instance = NULL;
std::map<int, CTexture*> CTextureManager::textures;

CTextureManager::~CTextureManager()
{
	cleanup();
}

CTextureManager::CTextureManager(int texCnt)
{
	texturesCount = texCnt;
	instance = NULL;
}

CTextureManager* CTextureManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new CTextureManager();
	}
	return instance;
}

void CTextureManager::cleanup()
{
	for each (auto currPair in textures)
	{
		CTexture* currTex = currPair.second;
		if (currTex != NULL)
		{			
			delete currTex;
			currTex = NULL;
		}
	}
	textures.clear();
}

CTexture* CTextureManager::getTexture(int texID)
{
	return textures[texID];
}

void CTextureManager::delTexture(int texID)
{
	if (textures[texID] != NULL) delete textures[texID];
	textures[texID] = NULL;
}

CTexture* CTextureManager::loadTexture(int texID, std::string fileName)
{
	if (textures[texID] != NULL) delete textures[texID];
	textures[texID] = new CTexture(fileName);
	return textures[texID];	
}

void CTextureManager::addTexture(int texID, CTexture* tex)
{
	if (textures[texID] != NULL) delete textures[texID];
	textures[texID] = tex;	
}
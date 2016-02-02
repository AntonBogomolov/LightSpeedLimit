#include "ResourceManager.h"
#include "global.h"
#include "GLUtils.h"

#include "TextureAtlasManager.h"
#include "TextureManager.h"
#include "EventsManager.h"
#include "ShaderManager.h"
#include "Log.h"

#include <fstream>

CTextureManager*		CResourceManager::textureManager = NULL;
CTextureAtlasManager*	CResourceManager::textureAtlasManager = NULL;
CShaderManager*			CResourceManager::shaderManager = NULL;
CEventsManager*			CResourceManager::eventsManager = NULL;
CGLUtils*				CResourceManager::glUtils = NULL;

CResourceManager*		CResourceManager::instance = NULL;

CResourceManager::CResourceManager()
{
	initManagers();
}


CResourceManager::~CResourceManager()
{
	cleanupManagers();
}

CResourceManager* CResourceManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new CResourceManager();
	}
	return instance;
}

void CResourceManager::initManagers()
{
	glUtils = CGLUtils::GetInstance();
	textureManager = CTextureManager::getInstance();
	textureAtlasManager = CTextureAtlasManager::getInstance();
	shaderManager = CShaderManager::getInstance();
	eventsManager = CEventsManager::getInstance();
}

void CResourceManager::cleanupManagers()
{
	CTextureManager::cleanup();
	CTextureAtlasManager::cleanup();
	CShaderManager::cleanup();
	CEventsManager::cleanup();
}

std::string CResourceManager::loadTextFile(std::string fileName)
{
	std::string fileText = "";
	
	unsigned int nBytes;
	std::ifstream textFile(fileName, std::ios::binary);
	if (!textFile)
	{
		CLog::getInstance()->addError(std::string("CResourceManager::loadTextFile() \t| Cant load file: ") + fileName);
		return "";
	}
	textFile.seekg(0, std::ios::end);
	nBytes = textFile.tellg();
	textFile.seekg(0, std::ios::beg);
	char* psz = new char[nBytes + 1];
	textFile.read(psz, nBytes);
	psz[nBytes] = '\0';
	fileText = psz;

	psz[nBytes] = 0;
	textFile.close();
	delete[] psz;

	//replaceAll(fileText, "/n", "");
	//replaceAll(fileText, "/r", "");

	return fileText;
}

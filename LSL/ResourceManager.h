#pragma once

#include <string>

class CTextureManager;
class CTextureAtlasManager;
class CShaderManager;
class CEventsManager;
class CGLUtils;

class CResourceManager
{
private:
	CResourceManager();
	//static Context context;
	//static AssetManager assets;	
public:
	static CTextureManager* textureManager;
	static CTextureAtlasManager* textureAtlasManager;
	static CShaderManager* shaderManager;
	static CEventsManager* eventsManager;
	//static CSoundManager soundManager;
	static CGLUtils* glUtils;

	static CResourceManager* instance;

	void initManagers();
	void cleanupManagers();
	static std::string loadTextFile(std::string fileName);
	//static Bitmap loadImage(String fileName)

	static CResourceManager* getInstance();
		
	~CResourceManager();
};


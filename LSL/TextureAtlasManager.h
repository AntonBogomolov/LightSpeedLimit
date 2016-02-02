#pragma once

#include <string>
#include <map>

class CTextureAtlas;

class CTextureAtlasManager
{
private:
	static int atlasCount;
	static CTextureAtlasManager* instance;

	CTextureAtlasManager(int shCnt = atlasCount);

public:
	//static CTextureAtlas** textureAtlas;
	static std::map<int, CTextureAtlas*> textureAtlas;

public:
	static CTextureAtlasManager* getInstance();	
	static void cleanup();
	static void lazyCleanup();
	static void cleanupUnused();

	static CTextureAtlas* getAtlas(int atlID);
	static CTextureAtlas* addAtlas(int atlID);
	static void delAtlas(int atlID);
	static void generateAllAtlases();

	~CTextureAtlasManager();
};


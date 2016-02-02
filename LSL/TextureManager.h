#pragma once
#include <string>
#include <map>

class CTexture;

class CTextureManager
{
private:
	static int texturesCount;
	static CTextureManager* instance;

	CTextureManager(int shCnt = texturesCount);

public:
//	static CTexture** textures;
	static std::map<int, CTexture*> textures;

public:
	static CTextureManager* getInstance();
	static void cleanup();

	static CTexture* getTexture(int texID);
	static void delTexture(int texID);
	static CTexture* loadTexture(int texID, std::string fileName);
	static void	addTexture(int texID, CTexture* tex);
	
	~CTextureManager();
};



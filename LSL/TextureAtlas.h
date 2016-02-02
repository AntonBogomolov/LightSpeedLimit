#pragma once

#include <vector>

class CTexture;
class CElasticBox;
class CTexCoord;

class CTextureAtlas
{
private:
	CElasticBox* coordBox;
	CTexture* tAtlas;
	std::vector<CTexCoord*> *texturesID;
	bool isUsing;
	bool isNeedToGenerateAtlas;
public:
	void cleanup();
	const CTexCoord* getTexCoord(const int texId)const;
	void addTexture(const std::string filename, const int texId);
	void addTexture(CTexture* tex, const int texId);
	void addSpriteSet(const std::string filename, const int texId, const int animSetCnt, const int animSpritesCnt);
	void addSpriteSet(CTexture* tex, const int texId, const int animSetCnt, const int animSpritesCnt);
	void generateAtlas();

	const CTexture* getAtlasTexture()const;
	const bool getIsUsing() const;
	void setIsUsing(const bool isUsing);

	CTextureAtlas();
	~CTextureAtlas();
};

class CTexCoord
{
public:
	int x;
	int y;
	int width;
	int height;
	float tx;
	float ty;
	float twidth;
	float theight;
	int texID;

	int setCnt;
	int spritesCnt;
	CTexCoord*** setCoord;

	CTexCoord();
	~CTexCoord();

	void setSpriteSetParam(const int setCnt, const int spritesCnt);
	const bool isSet()const;
	const CTexCoord* getSprite(const int spriteSet, const int spriteNumInSet)const;
	void initSubTexData();
	void setSubTexCoordData();
};

class CTexElasticBox
{
public:
	int x;
	int y;
	int w;
	int h;

	CTexElasticBox()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	CTexElasticBox(int x, int y, int w, int h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}
};

class CElasticBox
{
private:
	int item_w;
	int item_h;

	int x;
	int y;
	int w;
	int h;

	std::vector<CTexElasticBox*> childrens;
private:
	bool hitTest(const int x, const int y);
	bool testSpace(const int x, const int y, const int w, const int h);
	const int allocateSpace(int &x, int &y, const int itemW, const int itemH);
public:
	static const int FIND_SPACE_OK = 0;
	static const int FIND_SPACE_NEED_MORE = 1;
	static const int FIND_SPACE_NEED_WIDTH = 2;
	static const int FIND_SPACE_NEED_HEIGHT = 3;

	CElasticBox();
	~CElasticBox();

	void cleanup();
	
	void resize(const int newW, const int newH);
	
	const int getWidth()const;
	const int getHeight()const;
	void insertItem(int &x, int &y, const int itemW, const int itemH);
};
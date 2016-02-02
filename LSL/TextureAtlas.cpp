#include "TextureAtlas.h"
#include "global.h"
#include "Texture.h"
#include "GLUtils.h"
#include "TextureManager.h"
#include "Log.h"

CTextureAtlas::CTextureAtlas()
{
	texturesID = new std::vector<CTexCoord*>();
	coordBox = new CElasticBox();
	isUsing = true;
	isNeedToGenerateAtlas = true;
}


CTextureAtlas::~CTextureAtlas()
{
	cleanup();
}

void CTextureAtlas::cleanup()
{
	if (texturesID != NULL)
	{
		for each (CTexCoord* currCoord in (*texturesID))
		{
			delete currCoord;
		}
		texturesID->clear();
		delete texturesID;
		texturesID = NULL;
	}
	if (coordBox != NULL) delete coordBox;
	coordBox = NULL;
	if (tAtlas != NULL) delete tAtlas;
	tAtlas = NULL;
}

const CTexCoord* CTextureAtlas::getTexCoord(const int texId)const
{
	if (texId < 0) return NULL;

	for (unsigned int i = 0; i < texturesID->size(); i++)
	{
		if ((*texturesID)[i]->texID == texId) return (*texturesID)[i];
	}
	return NULL;
}

void CTextureAtlas::addTexture(const std::string filename, const int texId)
{
	for each (CTexCoord* currTexDescr in *texturesID)
	{
		if (currTexDescr->texID == texId) return;
	}

	int x;
	int y;

	CTexture* texture = CTextureManager::loadTexture(texId, filename);
	coordBox->insertItem(x, y, texture->getW(), texture->getH());

	CTexCoord* textureCoord = new CTexCoord();
	textureCoord->texID = texId;
	textureCoord->x = x;
	textureCoord->y = y;
	textureCoord->height = texture->getH();
	textureCoord->width = texture->getW();

	texturesID->push_back(textureCoord);
	isNeedToGenerateAtlas = true;	
}

void CTextureAtlas::addTexture(CTexture* tex, const int texId)
{
	for each (CTexCoord* currTexDescr in *texturesID)
	{
		if (currTexDescr->texID == texId) return;
	}
	
	int x;
	int y;

	CTextureManager::addTexture(texId, tex);
	coordBox->insertItem(x, y, tex->getW(), tex->getH());

	CTexCoord* textureCoord = new CTexCoord();
	textureCoord->texID = texId;
	textureCoord->x = x;
	textureCoord->y = y;
	textureCoord->height = tex->getH();
	textureCoord->width = tex->getW();

	texturesID->push_back(textureCoord);
	isNeedToGenerateAtlas = true;
}

void CTextureAtlas::addSpriteSet(const std::string filename, const int texId, const int animSetCnt, const int animSpritesCnt)
{
	for (auto it = texturesID->begin(); it != texturesID->end(); it++)
	{
		if ((*it)->texID == texId)
		{
			if ((*it)->setCnt == animSetCnt && (*it)->spritesCnt == animSpritesCnt) return;

			texturesID->erase(it);
			break;
		}
	}

	int x;
	int y;

	CTexture* texture = CTextureManager::loadTexture(texId, filename);
	coordBox->insertItem(x, y, texture->getW(), texture->getH());

	CTexCoord* textureCoord = new CTexCoord();
	textureCoord->texID = texId;
	textureCoord->x = x;
	textureCoord->y = y;
	textureCoord->height = texture->getH();
	textureCoord->width = texture->getW();

	textureCoord->setSpriteSetParam(animSetCnt, animSpritesCnt);
	textureCoord->initSubTexData();

	texturesID->push_back(textureCoord);
	isNeedToGenerateAtlas = true;	
}

void CTextureAtlas::addSpriteSet(CTexture* tex, const int texId, const int animSetCnt, const int animSpritesCnt)
{
	for (auto it = texturesID->begin(); it != texturesID->end(); it++)
	{
		if ((*it)->texID == texId)
		{
			if ((*it)->setCnt == animSetCnt && (*it)->spritesCnt == animSpritesCnt) return;

			texturesID->erase(it);
			break;
		}
	}

	int x;
	int y;	

	CTextureManager::addTexture(texId, tex);
	coordBox->insertItem(x, y, tex->getW(), tex->getH());

	CTexCoord* textureCoord = new CTexCoord();
	textureCoord->texID = texId;
	textureCoord->x = x;
	textureCoord->y = y;
	textureCoord->height = tex->getH();
	textureCoord->width  = tex->getW();

	textureCoord->setSpriteSetParam(animSetCnt, animSpritesCnt);
	textureCoord->initSubTexData();

	texturesID->push_back(textureCoord);
	isNeedToGenerateAtlas = true;	
}

void CTextureAtlas::generateAtlas()
{
	if (!isUsing || !isNeedToGenerateAtlas) return;	
	delete tAtlas;	
	
	int textureSide = coordBox->getWidth();
	if (textureSide < coordBox->getHeight()) textureSide = coordBox->getHeight();

	byte* pixels = new byte[textureSide * textureSide * 4]();
	tAtlas = new CTexture(GL_RGBA8, textureSide, textureSide, pixels);
	tAtlas->bind();

	for each (CTexCoord* currTex in (*texturesID))
	{
		CTextureManager::getTexture(currTex->texID)->bind();
		byte* buff = CGLUtils::glGetTexImage(currTex->texID, currTex->width, currTex->height);

		tAtlas->bind();
		glTexSubImage2D(GL_TEXTURE_2D, 0, currTex->x, currTex->y, currTex->width, currTex->height, GL_RGBA, GL_UNSIGNED_BYTE, buff);

		delete[] buff;
		buff = NULL;

		currTex->tx = (float)(currTex->x + 1) / textureSide;
		currTex->ty = (float)(currTex->y + 1) / textureSide;

		currTex->twidth = (float)(currTex->width - 2) / textureSide;
		currTex->theight = (float)(currTex->height - 2) / textureSide;

		currTex->setSubTexCoordData();
		CTextureManager::delTexture(currTex->texID);

		CTexture::unbind();
	}
	CLog::getInstance()->addInfo("CTextureAtlas::generateAtlas() \t| Created athlas texture: " + std::to_string(textureSide) + "x" + std::to_string(textureSide));

	delete[] pixels;
	pixels = NULL;
	isNeedToGenerateAtlas = false;

	CTexture::unbind();
}

const CTexture* CTextureAtlas::getAtlasTexture()const
{
	return tAtlas;
}

const bool CTextureAtlas::getIsUsing() const
{
	return isUsing;
}

void CTextureAtlas::setIsUsing(const bool isUsing)
{
	this->isUsing = isUsing;
}

//////////////////////////////////////////////////////////////////////////////////////////////

CTexCoord::CTexCoord()
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
	tx = 0;
	ty = 0;
	twidth = 0;
	theight = 0;
	texID = 0;

	setCnt = 0;
	spritesCnt = 0;
	setCoord = NULL;
}

CTexCoord::~CTexCoord()
{
	if (setCoord)
	{
		for (int i = 0; i < spritesCnt; i++)
		{
			for (int j = 0; j < setCnt; j++)
			{
				delete setCoord[i][j];
			}
			delete[] setCoord[i];
		}
		delete[] setCoord;
	}
}

void CTexCoord::setSpriteSetParam(const int setCnt, const int animSpritesCnt)
{
	if (setCnt <= 0) return;
	if (animSpritesCnt <= 0) return;

	this->setCnt = setCnt;
	this->spritesCnt = animSpritesCnt;

	setCoord = new CTexCoord**[this->spritesCnt]();
	for (int i = 0; i < this->spritesCnt; i++)
	{
		setCoord[i] = new CTexCoord*[this->setCnt]();
		for (int j = 0; j < this->setCnt; j++)
		{
			setCoord[i][j] = new CTexCoord();
		}
	}
}

const bool CTexCoord::isSet()const
{
	return (setCoord != NULL);
}

const CTexCoord* CTexCoord::getSprite(const int spriteSet, const int spriteNumInSet)const
{
	if (setCoord == NULL) return NULL;
	if (spriteSet > setCnt) return NULL;
	if (spriteNumInSet > spritesCnt) return NULL;

	return setCoord[spriteNumInSet][spriteSet];
}

void CTexCoord::initSubTexData()
{
	if (setCoord == NULL) return;
	if (width == 0) return;
	if (height == 0) return;

	int pixCntWidth = width / spritesCnt;
	int pixCntHeight = height / setCnt;
	for (int i = 0; i < spritesCnt; i++)
	{
		for (int j = 0; j < setCnt; j++)
		{
			setCoord[i][j]->x = x + i * pixCntWidth;
			setCoord[i][j]->y = y + j * pixCntHeight;
			setCoord[i][j]->width = pixCntWidth;
			setCoord[i][j]->height = pixCntHeight;
			setCoord[i][j]->texID = texID;
		}
	}
}

void CTexCoord::setSubTexCoordData()
{
	if (setCoord == NULL) return;
	if (twidth == 0) return;
	if (theight == 0) return;

	float spriteTWidth = twidth / spritesCnt;
	float spriteTHeight = theight / setCnt;

	for (int i = 0; i < spritesCnt; i++)
	{
		for (int j = 0; j < setCnt; j++)
		{
			setCoord[i][j]->tx = tx + i * spriteTWidth;
			setCoord[i][j]->ty = ty + j * spriteTHeight;
			setCoord[i][j]->twidth = spriteTWidth;
			setCoord[i][j]->theight = spriteTHeight;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////

CElasticBox::CElasticBox()
{
	item_w = 0;
	item_h = 0;
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

CElasticBox::~CElasticBox()
{
	cleanup();	
}

void CElasticBox::cleanup()
{
	for each (CTexElasticBox* currTexBox in childrens)
	{
		delete currTexBox;
	}
	childrens.clear();
}

CTexElasticBox* childrens;
	
void CElasticBox::resize(const int newW, const int newH)
{
	w = newW;
	h = newH;
}

const int CElasticBox::allocateSpace(int &x, int &y, const int itemW, const int itemH)
{
	int space_found = FIND_SPACE_OK;
	
	if (itemW > w && itemH > h) return FIND_SPACE_NEED_MORE;
	if (itemW > w) return FIND_SPACE_NEED_WIDTH;
	if (itemH > h) return FIND_SPACE_NEED_HEIGHT;

	x = 0;
	y = 0;

	int minTexW = itemW;
	int minTexH = itemH;
	for each (CTexElasticBox* currTexBox in childrens)
	{
		if (currTexBox->w < minTexW)minTexW = currTexBox->w;
		if (currTexBox->h < minTexH)minTexH = currTexBox->h;
	}

	for (int cy = 0; cy < h; cy += minTexH)
	{
		for (int cx = 0; cx < w; cx += minTexW)
		{
			if (testSpace(cx, cy, itemW, itemH))
			{
				childrens.push_back(new CTexElasticBox(cx, cy, itemW, itemH));
				x = cx;
				y = cy;
				return FIND_SPACE_OK;
			}
		}
	}

	if (w < h) return FIND_SPACE_NEED_WIDTH;
	else return FIND_SPACE_NEED_HEIGHT;
}

bool CElasticBox::hitTest(const int x, const int y)
{
	for each (CTexElasticBox* currTexBox in childrens)
	{
		if (currTexBox->x <= x && currTexBox->x + currTexBox->w > x &&
			currTexBox->y <= y && currTexBox->y + currTexBox->h > y) return true;
	}
	return false;
}

bool CElasticBox::testSpace(const int x, const int y, const int w, const int h)
{
	if (y + h > this->h) return false;
	if (x + w > this->w) return false;

	for (int i = y; i < y+h; i += 32)
	{
		for (int j = x; j < x+w; j += 32)
		{			
			if (hitTest(j, i)) return false;			
		}
	}
	return true;
}

const int CElasticBox::getWidth()const
{
	return w;
}

const int CElasticBox::getHeight()const
{
	return h;
}

void CElasticBox::insertItem(int &x, int &y, const int itemW, const int itemH)
{
	int space_found;
	while ((space_found = allocateSpace(x, y, itemW, itemH)) != FIND_SPACE_OK)
	{
		switch (space_found)
		{
		case CElasticBox::FIND_SPACE_NEED_WIDTH:
			resize(w + itemW, h);
			break;
		case CElasticBox::FIND_SPACE_NEED_HEIGHT:
			resize(w, h + itemH);
			break;
		default:
			resize(w + itemW, h + itemH);
		}
	}
}
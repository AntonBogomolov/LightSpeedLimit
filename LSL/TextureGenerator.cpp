#include "TextureGenerator.h"
#include <stdlib.h> 


CTextureGenerator::CTextureGenerator()
{

}

CTextureGenerator::~CTextureGenerator()
{

}

CHeightMap* CTextureGenerator::startDiamondSquareGenerator(const unsigned int sideLen, const unsigned int seed)
{
	CHeightMap* heightMap = new CHeightMap(sideLen, sideLen);
	srand(seed);

	for (unsigned int x = 0; x < sideLen; x++)
	{
		for (unsigned int y = 0; y < sideLen; y++)
		{
			heightMap->put(x, y, 0.0f);			
		}
	}

	float randValue = (float)(rand() % 1000) / 1000.0f;
	(*heightMap)[0] = randValue;
	randValue = (float)(rand() % 1000) / 1000.0f;
	(*heightMap)[sideLen - 1] = randValue;
	randValue = (float)(rand() % 1000) / 1000.0f;
	(*heightMap)[sideLen * (sideLen - 1)] = randValue;
	randValue = (float)(rand() % 1000) / 1000.0f;
	(*heightMap)[sideLen * sideLen - 1] = randValue;

	for (unsigned int squareLen = sideLen; squareLen > 2; squareLen /= 2)
	{
		diamondSquareStep(heightMap, squareLen);
	}

	return heightMap;
}

void CTextureGenerator::diamondSquareStep(CHeightMap* heightMap, const unsigned int squareLen)
{
	float leftTopHeight;
	float rightTopHeight;
	float leftBottomHeight;
	float rightBottomHeight;

	int midX;
	int midY;
	float midHeight;

	const unsigned int sideLen = heightMap->getWidth();

	for (unsigned int x = 0; x < sideLen / squareLen; x++)
	{
		midX = x * squareLen + squareLen / 2;
		for (unsigned int y = 0; y < sideLen / squareLen; y++)
		{
			midY = y * squareLen + squareLen / 2;

			int left = squareLen - 1;
			int bottom = squareLen - 1;
		
			leftTopHeight		= (*heightMap)[(y*squareLen + bottom) * sideLen + x * squareLen];
			rightTopHeight		= (*heightMap)[(y*squareLen + bottom) * sideLen + x * squareLen + left];
			leftBottomHeight	= (*heightMap)[(y*squareLen) * sideLen + x * squareLen];
			rightBottomHeight	= (*heightMap)[(y*squareLen) * sideLen + x * squareLen + left];

			float randMod = ((500.0f - rand() % 1000) / (sideLen / squareLen)) / 1000.0f;
			midHeight = (leftTopHeight + rightTopHeight + leftBottomHeight + rightBottomHeight) / 4.0f + randMod;
			if (midHeight > 1.0f) midHeight = 1.0f;
			if (midHeight < 0.0f) midHeight = 0.0f;

			(*heightMap)[(midY - 1) * sideLen + midX - 1]	= midHeight;
			(*heightMap)[(midY	  ) * sideLen + midX - 1]	= midHeight;
			(*heightMap)[(midY - 1) * sideLen + midX	]	= midHeight;
			(*heightMap)[(midY	  ) * sideLen + midX	]	= midHeight;
		}
	}

	for (unsigned int x = 0; x < sideLen / squareLen; x++)
	{		
		for (unsigned int y = 0; y < sideLen / squareLen; y++)
		{
			midX = x * squareLen;
			midY = y * squareLen + squareLen / 2;
			diamondStep(heightMap, squareLen, midX, midY, 0);
			midX = x * squareLen + squareLen / 2;
			midY = y * squareLen;
			diamondStep(heightMap, squareLen, midX, midY, 1);
			midX = x * squareLen + squareLen;
			midY = y * squareLen + squareLen / 2;
			diamondStep(heightMap, squareLen, midX, midY, 0);
			midX = x * squareLen + squareLen / 2;
			midY = y * squareLen + squareLen;
			diamondStep(heightMap, squareLen, midX, midY, 1);
		}
	}
}

void CTextureGenerator::diamondStep(CHeightMap* heightMap, const unsigned int squareLen, const unsigned int midX, const unsigned int midY, const int mod)
{
	int top;
	int bottom;
	int left;
	int right;

	float topHeight;
	float bottomHeight;
	float leftHeight;
	float rightHeight;

	top			= midY + squareLen / 2 - 1;
	bottom		= midY - squareLen / 2;
	left		= midX - squareLen / 2;
	right		= midX + squareLen / 2 - 1;

	const unsigned int sideLen = heightMap->getWidth();

	if (top >=	 (int)sideLen)		top		-= squareLen;
	if (right >= (int)sideLen)		right	-= squareLen;
	if (bottom	< 0)				bottom	+= squareLen;
	if (left	< 0)				left	+= squareLen;

	int x = midX - 1;
	if (x < 0) x = 0;
	int y = midY - 1;
	if (y < 0) y = 0;

	topHeight		= (*heightMap)[top		* sideLen + x];
	bottomHeight	= (*heightMap)[bottom	* sideLen + x];
	leftHeight		= (*heightMap)[y * sideLen + left];
	rightHeight		= (*heightMap)[y * sideLen + right];

	float randMod = ((500.0f - rand() % 1000) / (sideLen / squareLen)) / 1000.0f;
	float midHeight = (topHeight + bottomHeight + leftHeight + rightHeight) / 4.0f + randMod;
	if (midHeight > 1.0f) midHeight = 1.0f;
	if (midHeight < 0.0f) midHeight = 0.0f;

	if (mod == 0)
	{
		(*heightMap)[(y + 1) * sideLen + x] = midHeight;
		(*heightMap)[y * sideLen + x]		= midHeight;
		if (x + 1 < (int)sideLen)
		{
			(*heightMap)[(y + 1) * sideLen + x + 1] = midHeight;
			(*heightMap)[y * sideLen + x + 1]		= midHeight;
		}
	}
	else
	{
		(*heightMap)[y * sideLen + x + 1]	= midHeight;
		(*heightMap)[y * sideLen + x]		= midHeight;
		if (y + 1 < (int)sideLen)
		{
			(*heightMap)[(y + 1) * sideLen + x + 1] = midHeight;
			(*heightMap)[(y + 1) * sideLen + x]		= midHeight;
		}
	}
}
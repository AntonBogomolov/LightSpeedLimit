#include "PlanetTextureGenerator.h"

#include "Texture.h"
#include "TextureManager.h"
#include "TextureAtlas.h"
#include "TextureAtlasManager.h"
#include "Log.h"
#include "Planet.h"

CPlanetTextureGenerator::CPlanetTextureGenerator()
{
	genThread1 = NULL;
	genThread2 = NULL;
	genThread3 = NULL;
	genThread4 = NULL;

	resetGeneratorParams();
}

CPlanetTextureGenerator::~CPlanetTextureGenerator()
{
	delete genThread1;
	delete genThread2;
	delete genThread3;
	delete genThread4;
}

void CPlanetTextureGenerator::resetGeneratorParams()
{
	currPlanetParams	= NULL;
	hasCloudTexture		= true;
	hasLiquidTexture	= true;
	hasLumTexture		= true;
}

float CPlanetTextureGenerator::getWetness(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const
{
	const unsigned int sideLen = heightMap->getWidth();
	const float waterLevel = heightMap->getBorderValue(currPlanetParams->liquidPart);
	
	const float* heightMapData = heightMap->getDataPtr();
	if (heightMapData[y*sideLen + x] <= waterLevel) return 1.0f;

	const unsigned int areaToSearchLiquid = sideLen / 8;
	float wetness = 0.0f;
			
	unsigned int leftBorder = x - areaToSearchLiquid;
	if (x < areaToSearchLiquid) leftBorder = 0;	
	unsigned int bottomBorder = y - areaToSearchLiquid;
	if (y < areaToSearchLiquid) bottomBorder = 0;	

	unsigned int lenX = 2 * areaToSearchLiquid;
	if (lenX + leftBorder >= sideLen) lenX = sideLen - leftBorder - 1;
	unsigned int lenY = 2 * areaToSearchLiquid;
	if (lenY + bottomBorder >= sideLen) lenY = sideLen - bottomBorder - 1;

	const unsigned int pointsCnt = areaToSearchLiquid / 4;
	unsigned int sum = 0;
	for (unsigned int i = 0; i < pointsCnt; i++)
	{
		unsigned int cx = leftBorder + rand() % lenX;
		unsigned int cy = bottomBorder + rand() % lenY;

		if (heightMapData[cy*sideLen + cx] < waterLevel) sum += 1;
	}

	if (sum > pointsCnt / 8)
	{
		wetness = 1.0f;
	}
	else
	{
		wetness = sum / (pointsCnt / 8.0f);
	}

	return wetness;
}

float CPlanetTextureGenerator::getTemp(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const
{
	const unsigned int sideLen = heightMap->getWidth();

	float cx = x - sideLen / 2.0f;
	float cy = y - sideLen / 2.0f;
	float distance = sqrtf(cx*cx + cy*cy);
	float distanceFactor = distance / (sideLen / 2.0f);
	float tempVariation = 30.0f;

	float temp = (distanceFactor - 0.5f) * 2.0f * tempVariation + currPlanetParams->planetTemp;	
	temp -= heightMap->get(x, y) * tempVariation * 2.0f;
	if (temp < 0.0f) temp += (rand() % (int)tempVariation - tempVariation / 2.0f) * distanceFactor;

	return temp;
}

glm::fvec3 CPlanetTextureGenerator::getTerrainColor(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const
{
	float waterLevel = heightMap->getBorderValue(currPlanetParams->liquidPart);
	float terrHeight = 0;
	glm::fvec3 currColor = glm::fvec3(0.0f);

	const unsigned int sideLen = heightMap->getWidth();

	float cx = x - sideLen / 2.0f;
	float cy = y - sideLen / 2.0f;
	float distance = sqrtf(cx*cx + cy*cy);
	if (distance > sideLen / 2) return currColor;
		
	terrHeight = heightMap->get(x,y) - waterLevel;
	float slope = heightMap->getSlope(x, y);

	if (terrHeight <= 0.0f) currColor = currPlanetParams->liquidColor;
	else
	if (terrHeight < 0.05f) currColor = currPlanetParams->terrainLowColor;
	else
	if (terrHeight < 0.350f) currColor = currPlanetParams->terrainMedColor;
	else
	if (terrHeight <= 1.0f) currColor = currPlanetParams->terrainHightColor;

	float wetness = getWetness(x, y, heightMap); 
	float temp    = getTemp(x, y, heightMap);

	if (wetness > 0.1f && temp < 0.0f && temp > -200.0f)	currColor = currPlanetParams->iceColor;
	
	if (terrHeight <= 0.0f) currColor += slope * 0.5f;
	else currColor += slope * 2.0f;

	if (currColor.r > 1.0f) currColor.r = 1.0f;
	if (currColor.g > 1.0f) currColor.g = 1.0f;
	if (currColor.b > 1.0f) currColor.b = 1.0f;
	if (currColor.r < 0.0f) currColor.r = 0.0f;
	if (currColor.g < 0.0f) currColor.g = 0.0f;
	if (currColor.b < 0.0f) currColor.b = 0.0f;

	return currColor;
}

glm::fvec4 CPlanetTextureGenerator::getCloudColor(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const
{
	const unsigned int sideLen = heightMap->getWidth();
	float cloudPart = heightMap->getBorderValue(currPlanetParams->cloudPart);	
	float cloudHeight = heightMap->get(x, y) - cloudPart;

	glm::fvec4 currColor = glm::fvec4(0.0f);

	float cx = x - sideLen / 2.0f;
	float cy = y - sideLen / 2.0f;
	float distance = sqrtf(cx*cx + cy*cy);
	if (distance > sideLen / 2) return currColor;

	if (cloudHeight > 0.0f) currColor = glm::fvec4(0.0f, 0.0f, 0.0f, 0.0f);
	else
	{
		float slope = heightMap->getSlope(x, y);

		currColor.r = currPlanetParams->cloudColor.r;
		currColor.g = currPlanetParams->cloudColor.g;
		currColor.b = currPlanetParams->cloudColor.b;
		currColor.a = 0.8f - slope * 100;
	}

	if (currColor.r > 1.0f) currColor.r = 1.0f;
	if (currColor.g > 1.0f) currColor.g = 1.0f;
	if (currColor.b > 1.0f) currColor.b = 1.0f;
	if (currColor.a > 1.0f) currColor.a = 1.0f;
	if (currColor.r < 0.0f) currColor.r = 0.0f;
	if (currColor.g < 0.0f) currColor.g = 0.0f;
	if (currColor.b < 0.0f) currColor.b = 0.0f;
	if (currColor.a < 0.0f) currColor.a = 0.0f;

	return currColor;
}

bool CPlanetTextureGenerator::isWater(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const
{
	const float	waterLevel = heightMap->getBorderValue(currPlanetParams->liquidPart);
	const float	terrHeight = heightMap->get(x, y) - waterLevel;

	float temp = getTemp(x, y, heightMap);
	return	terrHeight < 0.0f && (temp > 0.0f || temp < -200.0f);
}

void CPlanetTextureGenerator::genThreadWorker(const CHeightMap* heightMap, unsigned char* pixels, const unsigned int textureWidth, int part)
{	
	const unsigned int width = heightMap->getWidth();
	const unsigned int bpp = 4;

	glm::fvec3 currColor;
	unsigned int startX = 0;
	unsigned int stopX  = width / 2;
	unsigned int startY = 0;
	unsigned int stopY  = width / 2;
	if (part == 2)
	{
		startX = width / 2;
		stopX  = width;
		startY = 0 ;
		stopY = width / 2;
	}
	if (part == 3)
	{
		startX = 0;
		stopX = width / 2;
		startY = width / 2;
		stopY = width;
	}
	if (part == 4)
	{
		startX = width / 2;
		stopX = width;
		startY = width / 2;
		stopY = width;
	}

	unsigned int y;
	unsigned int x;
	for ( y = startY; y < stopY; y++)
	{
		for ( x = startX; x < stopX; x++)
		{			
			currColor = getTerrainColor(x, y, heightMap);

			pixels[(y*textureWidth + x)*bpp + 0] = (int)(currColor.r * 255);
			pixels[(y*textureWidth + x)*bpp + 1] = (int)(currColor.g * 255);
			pixels[(y*textureWidth + x)*bpp + 2] = (int)(currColor.b * 255);
			pixels[(y*textureWidth + x)*bpp + 3] = 255;

			if (isWater(x, y, heightMap) && hasLiquidTexture)
			{
				unsigned int pos = (y * textureWidth + width + x) * bpp;
				if (hasCloudTexture) pos = (y * textureWidth + width * 2 + x) * bpp;
				
				pixels[pos + 0] = 255;
				pixels[pos + 3] = 255;
			}
		}
	}
	CLog::getInstance()->addInfo("genThread stoped " + std::to_string(x) + " " + std::to_string(y));
}

CTexture* CPlanetTextureGenerator::generateTexture(const unsigned int width, const unsigned int height, const int bpp)
{
	CTexture* planetTexture = NULL;

	int textureWidth = width;
	int textureHeight = height;

	if (hasCloudTexture) textureWidth += width;
	if (hasLumTexture || hasLiquidTexture)	textureWidth += width;

	unsigned char* pixels = new unsigned char[textureWidth * textureHeight * bpp];
	memset(pixels, 0, textureWidth * textureHeight * bpp);

	////////////////////////////////////////////////////////////////
	if (hasLiquidTexture)
	{
		for (unsigned int y = 0; y < width; y++)
		{
			for (unsigned int x = 0; x < width; x++)
			{
				unsigned int pos = (y*textureWidth + width + x)*bpp;
				if (hasCloudTexture) pos = (y*textureWidth + width * 2 + x)*bpp;
								
				pixels[pos + 0] = 0;
				pixels[pos + 1] = 0;
				pixels[pos + 2] = 0;
				pixels[pos + 3] = 0;
			}
		}
	}
	////////////////////////////////////////////////////////////////
	CHeightMap* heightMap = startDiamondSquareGenerator(width, currPlanetParams->seed);
	heightMap->pow(2.0f);
	heightMap->calcAverageValues();	
		
	delete genThread2;
	genThread2 = new std::thread(&CPlanetTextureGenerator::genThreadWorker, this, heightMap, pixels, textureWidth, 2);
	delete genThread3;
	genThread3 = new std::thread(&CPlanetTextureGenerator::genThreadWorker, this, heightMap, pixels, textureWidth, 3);
	delete genThread4;
	genThread4 = new std::thread(&CPlanetTextureGenerator::genThreadWorker, this, heightMap, pixels, textureWidth, 4);
	
	genThreadWorker(heightMap, pixels, textureWidth, 1);	

	genThread2->join();
	genThread3->join();
	genThread4->join();

	CLog::getInstance()->addInfo("Texture generation done");

	delete heightMap;
	///////////////////////////////////////////////////////////////////////	
	if (hasCloudTexture)
	{
		heightMap = startDiamondSquareGenerator(width, currPlanetParams->seed);
		heightMap->pow(0.1f);
		heightMap->blurHeightMap(3);
		heightMap->calcAverageValues();

		for (unsigned int y = 0; y < width; y++)
		{
			for (unsigned int x = width; x < 2 * width; x++)
			{
				glm::fvec4 currColor = getCloudColor(x - width, y, heightMap);

				pixels[(y*textureWidth + x)*bpp + 0] = (int)(currColor.r * 255);
				pixels[(y*textureWidth + x)*bpp + 1] = (int)(currColor.g * 255);
				pixels[(y*textureWidth + x)*bpp + 2] = (int)(currColor.b * 255);
				pixels[(y*textureWidth + x)*bpp + 3] = (int)(currColor.a * 255);
			}
		}
		delete heightMap;
	}
	///////////////////////////////////////////////////////////////////////

	planetTexture = new CTexture(GL_RGBA8, textureWidth, textureHeight, (GLubyte*)pixels);
	delete [] pixels;	

	return planetTexture;
}

CTexture* CPlanetTextureGenerator::generatePlanetTexture(const int planetTexWidth, const int planetTexHeight, const PlanetGenerationParams* params, const int bpp)
{
	float eps = 0.00001f;

	currPlanetParams = params;

	hasCloudTexture  = true;
	hasLumTexture    = true;
	hasLiquidTexture = true;

	if (currPlanetParams->cloudPart  < eps) hasCloudTexture	 = false;
	if (currPlanetParams->lumPart    < eps) hasLumTexture	 = false;
	if (currPlanetParams->liquidPart < eps) hasLiquidTexture = false;
	
	return 	generateTexture(planetTexWidth, planetTexHeight, bpp);
}
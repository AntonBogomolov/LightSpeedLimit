#pragma once
#include "TextureGenerator.h"
#include <thread>
#include <chrono>

#include "../../tools/includes/glm/glm.hpp"

struct PlanetGenerationParams;

class CPlanetTextureGenerator :	public CTextureGenerator
{
private:
	const PlanetGenerationParams* currPlanetParams;

	bool hasCloudTexture;
	bool hasLumTexture;
	bool hasLiquidTexture;

	std::thread* genThread1;
	std::thread* genThread2;
	std::thread* genThread3;
	std::thread* genThread4;
private:
	virtual CTexture* generateTexture(const unsigned int width, const unsigned int height, const int bpp = 4);

	void genThreadWorker(const CHeightMap* heightMap, unsigned char* pixels, const unsigned int textureWidth, int part);

	glm::fvec4 getCloudColor(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const;
	glm::fvec3 getTerrainColor(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const;
	float getWetness(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const;
	float getTemp(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const;
	bool  isWater(const unsigned int x, const unsigned int y, const CHeightMap* heightMap) const;
public:
	CTexture* generatePlanetTexture(const int planetTexWidth, const int planetTexHeight, const PlanetGenerationParams* params, const int bpp = 4);

	void resetGeneratorParams();

	CPlanetTextureGenerator();
	virtual ~CPlanetTextureGenerator();
};


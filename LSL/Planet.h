#pragma once
#include "SpaceObject.h"

struct PlanetGenerationParams
{
	float liquidPart;
	
	float forestPart;
	float lumPart;
	float cloudPart;

	float sharpness;
	float flat;
	float planetTemp;

	unsigned int seed;

	glm::fvec3 liquidColor;
	glm::fvec3 lumColor;
	glm::fvec3 forestColor;
	glm::fvec3 iceColor;
	glm::fvec3 cloudColor;
	glm::fvec3 terrainLowColor;
	glm::fvec3 terrainMedColor;
	glm::fvec3 terrainHightColor;

	PlanetGenerationParams()
	{
		liquidPart = 0.2f;
		forestPart = 0.1f;
		lumPart = 0.0f;		
		cloudPart = 0.4f;

		sharpness = 0.5f;
		flat = 0.8f;
		planetTemp = 20.0f;
		
		seed = (unsigned int)this;

		liquidColor = glm::fvec3(0.0f, 0.428f, 0.654f);
		lumColor = glm::fvec3(0.588f, 0.0f, 0.09f);
		forestColor = glm::fvec3(0.09f, 0.447f, 0.270f);
		iceColor = glm::fvec3(0.94f, 0.97f, 1.0f);
		cloudColor = glm::fvec3(0.92f, 0.825f, 0.98f);
		terrainLowColor = glm::fvec3(0.95f, 0.95f, 0.85f);
		terrainMedColor = glm::fvec3(0.309f, 0.474f, 0.258f);
		terrainHightColor = glm::fvec3(0.450f, 0.551f, 0.480f);
	}
};

class CPlanet :
	public CSpaceObject
{
public:
	static const int PLANET_TYPE_EARTHLIKE = 0;
	static const int PLANET_TYPE_GASGIANT = 1;
	static const int PLANET_TYPE_DWARF = 2;

	static double EARTH_MASS;
	static double EARTH_RAD;
	static const int EARTH_SPRITE_SIZE = 96;
	static const int PLANET_SPRITE_MAXSIZE = 256;
	static const int PLANET_SPRITE_MINSIZE = 16;
private:	
	int planetTemp;
	int planetType;
	double dayLength;
	double yearLength;

	PlanetGenerationParams* planetGeneratorParams;
public:
	const int getPlanetType()const;
	const int getPlanetTemp()const;
	const double getPlaneYearLength()const;
	const double getPlanetDayLength()const;

	void setPlanetType(const int planetType);
	void setPlanetTemp(const int planetTemp);
	void setPlanetGeneratorParams(PlanetGenerationParams* planetGeneratorParams);
	PlanetGenerationParams* getPlanetGeneratorParams() const;
	void calcPlanetGeneratorParams();

	virtual void updateShader();
	virtual void update(const float dt);
	virtual void setSprite(const int atlasId, const int textureId, const int setsCnt = 1, const int animCnt = 1);
	void setPlanetParams(const int planetType, const int planetTemp);
	void calcPlanetParametrs();	
protected:
	virtual const int calcSpriteSize();
public:
	CPlanet(const std::string name, const int planetType = PLANET_TYPE_EARTHLIKE, const int planetTemp = 28, const double x = 0, const double y = 0,
		const double mass = EARTH_MASS, const double radius = EARTH_RAD, const glm::dvec2 velocity = glm::dvec2(0.0f, 0.0f), const glm::dvec2 acceleration = glm::dvec2(0.0f, 0.0f));
	//CPlanet();
	virtual ~CPlanet();
};


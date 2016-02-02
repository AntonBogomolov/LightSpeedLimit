#include "Planet.h"
#include "AnimatedSprite.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "StarSystem.h"
#include "Star.h"
#include "Scene.h"
#include "GLShaderObject.h"
#include "ShaderWarper.h"
#include "AppTime.h"
#include "TextureAtlas.h"
#include "TextureAtlasManager.h"

double CPlanet::EARTH_MASS = 5.9726e6;
double CPlanet::EARTH_RAD = 6.371e6;

CPlanet::CPlanet(const std::string name, const int planetType, const int planetTemp, const double x, const double y,
				 const double mass, const double radius, const glm::dvec2 velocity, const glm::dvec2 acceleration) :
				 CSpaceObject(x, y, velocity, acceleration, mass, radius)
{
	setIsSmallMass(false);
	setSpaceObjName(name);
	setPlanetParams(planetType, planetTemp);
	setType(GameDefines::GAME_TYPE_PLANET);

	planetGeneratorParams = new PlanetGenerationParams();
	planetGeneratorParams->seed = (unsigned int)(this) + (unsigned int)&velocity;
}

CPlanet::~CPlanet()
{
	delete planetGeneratorParams;
}

const int CPlanet::getPlanetType()const
{
	return planetType;
}

const int CPlanet::getPlanetTemp()const
{
	return planetTemp;
}

const double CPlanet::getPlaneYearLength()const
{
	return yearLength;
}

const double CPlanet::getPlanetDayLength()const
{
	return dayLength;
}

void CPlanet::setPlanetType(const int planetType)
{
	this->planetType = planetType;
}

void CPlanet::setPlanetTemp(const int planetTemp)
{
	this->planetTemp = planetTemp;
}

void CPlanet::setPlanetParams(const int planetType, const int planetTemp)
{
	this->planetType = planetType;
	this->planetTemp = planetTemp;
}

void CPlanet::setPlanetGeneratorParams(PlanetGenerationParams* planetGeneratorParams)
{
	delete this->planetGeneratorParams;
	this->planetGeneratorParams = planetGeneratorParams;
}

PlanetGenerationParams* CPlanet::getPlanetGeneratorParams() const
{
	return this->planetGeneratorParams;
}

void CPlanet::calcPlanetGeneratorParams()
{
	planetGeneratorParams->planetTemp = (float)planetTemp;
	srand(planetGeneratorParams->seed);
	float paramVariation;	

	switch (planetType)	
	{
	case PLANET_TYPE_EARTHLIKE:
		if (planetTemp < -200)
		{
			paramVariation = (rand() % 30 - 15) * 0.01f;
			planetGeneratorParams->cloudPart  = 0.1f + paramVariation;
			planetGeneratorParams->liquidPart = 0.4f + paramVariation;
		}
		if (planetTemp < -50)
		{
			paramVariation = (rand() % 20 - 10) * 0.01f;
			planetGeneratorParams->cloudPart  = 0.2f + paramVariation;
			planetGeneratorParams->liquidPart = 0.1f + paramVariation;
		}
		if (planetTemp > 100)
		{
			paramVariation = (rand() % 60 - 30) * 0.01f;
			planetGeneratorParams->cloudPart  = 0.8f + paramVariation;
			planetGeneratorParams->liquidPart = 0.1f + paramVariation;
		}
		if (planetTemp > 200)
		{
			paramVariation = (rand() % 10 - 5) * 0.01f;
			planetGeneratorParams->cloudPart  = 0.0f + paramVariation;
			planetGeneratorParams->liquidPart = 0.0f + paramVariation;
		}

		if (planetGeneratorParams->cloudPart  < 0.0f) planetGeneratorParams->cloudPart  = 0.0f;
		if (planetGeneratorParams->liquidPart < 0.0f) planetGeneratorParams->liquidPart = 0.0f;
		if (planetGeneratorParams->cloudPart  > 1.0f) planetGeneratorParams->cloudPart  = 1.0f;
		if (planetGeneratorParams->liquidPart > 1.0f) planetGeneratorParams->liquidPart = 1.0f;

		break;
	case PLANET_TYPE_GASGIANT:

		break;
	case PLANET_TYPE_DWARF:

		break;
	default:
		break;
	}
	
}

void CPlanet::calcPlanetParametrs()
{

}

const int CPlanet::calcSpriteSize()
{
	int spriteSize = (int)((getRadius() / EARTH_RAD) * EARTH_SPRITE_SIZE);
	if (spriteSize > PLANET_SPRITE_MAXSIZE) spriteSize = PLANET_SPRITE_MAXSIZE;
	if (spriteSize < PLANET_SPRITE_MINSIZE) spriteSize = PLANET_SPRITE_MINSIZE;

	return spriteSize;
}

void CPlanet::setSprite(const int atlasId, const int textureId, const int setsCnt, const int animCnt)
{
	__super::setSprite(atlasId, textureId, setsCnt, animCnt);

	if (ndSpaceObjSprite)
	{		
		if (twTexturer)
		{
			twTexturer->setShader(CShaderManager::getShader(Defines::SH_PLANET_TEXTURER));

			if (planetGeneratorParams->cloudPart > 0.0f)	twTexturer->setShader(CShaderManager::getShader(Defines::SH_PLANET_TEXTURER_C));
			if (planetGeneratorParams->liquidPart > 0.0f)	twTexturer->setShader(CShaderManager::getShader(Defines::SH_PLANET_TEXTURER_L));
			if (planetGeneratorParams->cloudPart > 0.0f && planetGeneratorParams->liquidPart > 0.0f) twTexturer->setShader(CShaderManager::getShader(Defines::SH_PLANET_TEXTURER_CL));
		}
	}
}

void CPlanet::update(const float dt)
{
	__super::update(dt);
}

void CPlanet::updateShader()
{
	const void* param = getShaderUpdaterParam();
	if (param == NULL) return;
	CStarSystem* currSystem = (CStarSystem*)param;

	const std::vector<CStar*>* currStars = currSystem->getStars();
	CStar* currStar = NULL;
	for each (CStar* star in *currStars)
	{
		currStar = star;
	}

	if (currStar == NULL) return;
		
	glm::vec2 termVec = currStar->getPos() - this->getPos();
	float oldX = termVec.x;
	termVec.x = -termVec.y;
	termVec.y = oldX;
	termVec = glm::normalize(termVec);

	glm::fvec3 starColor = currStar->getStarColor();
	const CGLShaderObject* currShader = twTexturer->getShader();

	int atlId = spaceObjSprite->getSpriteAtlasID();
	int texId = spaceObjSprite->getSpriteTextureID();
	const CTextureAtlas* currAtlas = CTextureAtlasManager::getAtlas(atlId);
	if (!currAtlas) return;
	const CTexCoord* planetTex = currAtlas->getTexCoord(texId);
	if (!planetTex) return;
		
	float texModX = 1.0f;
	if (planetGeneratorParams->cloudPart > 0.0f && planetGeneratorParams->liquidPart > 0.0f) texModX = 3.0f;		
	else if (planetGeneratorParams->cloudPart > 0.0f || planetGeneratorParams->liquidPart > 0.0f) texModX = 2.0f;
	
	currShader->enable();
	currShader->setUniformParameter1f("time", CAppTime::getInstance()->getNow());
	currShader->setUniformParameter1f("rotSpeed", (float)(getRadius() / EARTH_RAD) * 0.4f);	
	currShader->setUniformParameter2f("normTerminator", termVec.x, termVec.y);
	currShader->setUniformParameter3f("starColor", starColor.r, starColor.g, starColor.b);	
	currShader->setUniformParameter2f("texSize",   planetTex->twidth / texModX, planetTex->theight);
	currShader->setUniformParameter2f("texCenter", planetTex->tx + planetTex->twidth / 2.0f, planetTex->ty + planetTex->theight / 2.0f);
	currShader->setUniformParameter1f("texSizeMod", texModX);

	if (planetGeneratorParams->cloudPart > 0.0f)
	{	
		currShader->setUniformParameter1f("cloudsSpeed", 0.2f);		
	}
	if (planetGeneratorParams->liquidPart > 0.0f)
	{
		currShader->setUniformParameter3f("lumColor", planetGeneratorParams->lumColor.r, planetGeneratorParams->lumColor.g, planetGeneratorParams->lumColor.b);
	}

	currShader->disable();

	setIsNeedToUpdateShader(true);
}
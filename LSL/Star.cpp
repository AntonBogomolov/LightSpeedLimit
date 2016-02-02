#include "Star.h"
#include "GLShaderObject.h"
#include "ShaderManager.h"
#include "ShaderWarper.h"

double CStar::SUN_MASS = 1.9891e13; //1.9891e30;
double CStar::SUN_LUM = 3.846e26;
double CStar::SUN_RAD = 6.960e8;

//CStar::CStar() : CSpaceObject()
//{
//	setIsSmallMass(false);
//	setSpaceObjName("unnamed_star");
//	setType(GameDefines::GAME_TYPE_STAR);
//	setStarParams(STAR_CLASS_G, SUN_TEMP, STAR_TYPE_SUN, SUN_LUM);
//}

CStar::CStar(const std::string name, const int starClass, const int starTemp, const int starType, const double starLum, const double x, const double y, 
			 const double mass, const double radius, const glm::dvec2 velocity, const glm::dvec2 acceleration)
			 : CSpaceObject(x, y,  velocity, acceleration, mass, radius)
{
	setIsSmallMass(false);
	setSpaceObjName(name);
	setStarParams(starClass,starTemp,starType,starLum);
	setType(GameDefines::GAME_TYPE_STAR);
}

CStar::~CStar()
{

}

const int CStar::getStarClass()const
{
	return starClass;
}

const int CStar::getStarTemp()const
{
	return starTemp;
}

const int CStar::getStarType()const
{
	return starType;
}

const double CStar::getStarLum()const
{
	return starLum;
}

const double CStar::getHabitableZoneRadius() const
{
	return sqrt(starLum / SUN_LUM) * 145.0;
}

const glm::fvec3 CStar::getStarColor() const
{
	return starColor;
}

void CStar::setStarClass(const int starClass)
{
	this->starClass = starClass;
	switch (this->starClass)
	{
	case STAR_CLASS_A:
		setStarColor(glm::fvec3(0.95f, 0.95f, 0.95f));
		break;
	case STAR_CLASS_B:
		setStarColor(glm::fvec3(0.8f, 0.8f, 0.95f));
		break;
	case STAR_CLASS_O:
		setStarColor(glm::fvec3(0.6f, 0.6f, 0.9f));
		break;
	case STAR_CLASS_F:
		setStarColor(glm::fvec3(0.9f, 0.9f, 0.9f));
		break;
	case STAR_CLASS_G:
		setStarColor(glm::fvec3(0.8f, 0.6f, 0.2f));
		break;
	case STAR_CLASS_K:
		setStarColor(glm::fvec3(0.9f, 0.6f, 0.1f));
		break;
	case STAR_CLASS_M:
		setStarColor(glm::fvec3(0.9f, 0.5f, 0.1f));
		break;
	default:
		break;
	}
}

void CStar::setStarTemp(const int starTemp)
{
	this->starTemp = starTemp;
}

void CStar::setStarType(const int starType)
{
	this->starType = starType;
}

void CStar::setStarLum(const double starLum)
{
	this->starLum = starLum;
}

void CStar::setStarColor(glm::fvec3 starColor)
{
	this->starColor = starColor;
}

void CStar::setStarParams(const int starClass, const int starTemp, const int starType, const double starLum)
{	
	setStarClass(starClass);
	setStarLum(starLum);
	setStarTemp(starTemp);
	setStarType(starType);
}

const int CStar::calcSpriteSize()
{
	int spriteSize = (int)((getRadius() / SUN_RAD) * SUN_SPRITE_SIZE);
	if (spriteSize > STAR_SPRITE_MAXSIZE) spriteSize = STAR_SPRITE_MAXSIZE;
	if (spriteSize < STAR_SPRITE_MINSIZE) spriteSize = STAR_SPRITE_MINSIZE;

	return spriteSize;
}

void CStar::setSprite(const int atlasId, const int textureId, const int setsCnt, const int animCnt)
{
	__super::setSprite(atlasId, textureId, setsCnt, animCnt);

	if (ndSpaceObjSprite)
	{
		if (twTexturer) twTexturer->setShader(CShaderManager::getShader(Defines::SH_STARS_TEXTURES));
	}
}

void CStar::update(const float dt)
{
	__super::update(dt);
}


void CStar::updateShader()
{	
	setIsNeedToUpdateShader(true);	

	twTexturer->getShader()->enable();
	twTexturer->getShader()->setUniformParameter3f("starColor", starColor.r, starColor.g, starColor.b);
	twTexturer->getShader()->disable();
}
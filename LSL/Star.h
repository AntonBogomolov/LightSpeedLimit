#pragma once
#include "SpaceObject.h"

class CStar : public CSpaceObject
{
public:
	static const int STAR_CLASS_O = 0;
	static const int STAR_CLASS_B = 1;
	static const int STAR_CLASS_A = 2;
	static const int STAR_CLASS_F = 3;
	static const int STAR_CLASS_G = 4;
	static const int STAR_CLASS_K = 5;
	static const int STAR_CLASS_M = 6;

	static const int STAR_TYPE_SUN = 0;

	static double SUN_MASS;
	static double SUN_LUM;
	static double SUN_RAD;
	static const int SUN_TEMP = 5778;
	static const int SUN_SPRITE_SIZE = 384;
	static const int STAR_SPRITE_MAXSIZE = 512;
	static const int STAR_SPRITE_MINSIZE = 32;
private:
	int starClass;
	int starTemp;
	int starType;
	double starLum;

	glm::fvec3 starColor;
public:
	const int getStarClass()const;
	const int getStarTemp()const;
	const int getStarType()const;
	const double getStarLum()const;
	const double getHabitableZoneRadius() const;
	const glm::fvec3 getStarColor() const;

	void setStarClass(const int starClass);
	void setStarTemp(const int starTemp);
	void setStarType(const int starType);
	void setStarLum(const double starLum);
	void setStarColor(const glm::fvec3 starColor);

	virtual void setSprite(const int atlasId, const int textureId, const int setsCnt = 1, const int animCnt = 1);
	virtual void updateShader();
	virtual void update(const float dt);
	void setStarParams(const int starClass, const int starTemp, const int starType, const double starLum);
protected:
	virtual const int calcSpriteSize();
public:
	CStar(const std::string name, const int starClass = STAR_CLASS_G, const int starTemp = SUN_TEMP, const int starType = STAR_TYPE_SUN, const double starLum = SUN_LUM, const double x = 0, const double y = 0,
		const double mass = SUN_MASS, const double radius = SUN_RAD, const glm::dvec2 velocity = glm::dvec2(0.0f, 0.0f), const glm::dvec2 acceleration = glm::dvec2(0.0f, 0.0f));
	//CStar();
	virtual ~CStar();
};


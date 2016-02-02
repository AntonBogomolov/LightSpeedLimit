#pragma once
#include "../../tools/includes/glm/glm.hpp"
#include <queue>

class CPhysicsObject;

class COrbit
{
private:
	//float r;
	//float v;

	//float a;
	//float e;
	//float i;
	//float w;
	//float sig;
	//float m0;

	//float E;
	//float b;

	//float getSemimajorAxis();
	//float getEccentricity();
	//float getInclination();
	//float getLongitudeOfTheAscendingNode();
	//float getArgumentOfPeriapsis();
	//float getTrueAnomaly();

	//float getSemimajorAxis();
	//float getEccentricity();

	double currTime;
	glm::dvec2 currVelocity;
	glm::dvec2 currAcceleration;
	glm::dvec2 currPos;

	static const int futureOrbitLength;
	static const int pastOrbitLength;
	bool isNeedToCalcOrbit;

	float* futureOrbit;
	float* pastOrbit;
	std::queue<float> pastOrbitQuery;

	CPhysicsObject* phyObj;		
	float coordScale;
public:
	void setIsNeedToCalcOrbit(bool isNeedToCalcOrbit);	
	void setCoordScale(float scale);

	const float* getFutureOrbitData() const;
	const int	 getFutureOrbitDataLength() const;
	static int	 getFutureOrbitStepsCnt();	
	const float* getPastOrbitData() const;
	const int	 getPastOrbitDataLength() const;
	static int	 getPastOrbitStepsCnt();

	bool    getIsNeedToCalcOrbit() const;	
	float	getCoordScale() const;

	//void calcFutureOrbit();
	void saveCurrPhyStateAsFutureOrbitData(const int index);
	void saveCurrPhyStateAsPastOrbitData();

	COrbit(CPhysicsObject* phyObj);
	~COrbit();
};


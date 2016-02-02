#include "Orbit.h"
#include "PhysicsObject.h"
#include "EngineSettings.h"

const int COrbit::futureOrbitLength = EngineSettings::orbitFutureLength;
const int COrbit::pastOrbitLength	= EngineSettings::orbitPastLength;

COrbit::COrbit(CPhysicsObject* phyObj)
{
	this->isNeedToCalcOrbit = true;

	this->phyObj = phyObj;
	if (this->phyObj == NULL)
	{
		this->isNeedToCalcOrbit = false;
	}

	this->futureOrbit = new float[futureOrbitLength * 3];	
	this->pastOrbit   = new float[pastOrbitLength * 3];
	this->coordScale = 1.0f;	
}

COrbit::~COrbit()
{
	if (futureOrbit) delete[] futureOrbit;
}

void COrbit::setIsNeedToCalcOrbit(bool isNeedToCalcOrbit)
{
	this->isNeedToCalcOrbit = isNeedToCalcOrbit;
}

const float* COrbit::getFutureOrbitData() const
{
	return futureOrbit;
}

const int COrbit::getFutureOrbitDataLength() const
{
	return futureOrbitLength * 3;
}

int	COrbit::getFutureOrbitStepsCnt()
{
	return futureOrbitLength;
}

const float* COrbit::getPastOrbitData() const
{
	unsigned int len = pastOrbitLength * 3;
	if (len > pastOrbitQuery.size()) len = pastOrbitQuery.size();

	auto container = pastOrbitQuery._Get_container();
	for (unsigned int i = 0; i < len; i += 3)
	{
		pastOrbit[i + 0] = container[(len - 1) - i - 2];
		pastOrbit[i + 1] = container[(len - 1) - i - 1];
		pastOrbit[i + 2] = container[(len - 1) - i - 0];
	}

	return pastOrbit;
}

const int COrbit::getPastOrbitDataLength() const
{
	return pastOrbitQuery.size();
}

int	 COrbit::getPastOrbitStepsCnt()
{
	return pastOrbitLength;
}

bool COrbit::getIsNeedToCalcOrbit() const
{
	return isNeedToCalcOrbit;
}

void COrbit::setCoordScale(float scale)
{
	coordScale = scale;
}

float COrbit::getCoordScale() const 
{
	return coordScale;
}

void COrbit::saveCurrPhyStateAsFutureOrbitData(const int index)
{
	if (!isNeedToCalcOrbit) return;
	if (index >= futureOrbitLength || index < 0) return;

	futureOrbit[index * 3 + 0] = (float)phyObj->GetCurrModelData()[phyObj->GetModelDim() - 1].x * coordScale;
	futureOrbit[index * 3 + 1] = (float)phyObj->GetCurrModelData()[phyObj->GetModelDim() - 1].y * coordScale;
	futureOrbit[index * 3 + 2] = 0.0f;
}

void COrbit::saveCurrPhyStateAsPastOrbitData()
{
	if (!isNeedToCalcOrbit) return;

	float x = (float)phyObj->GetCurrModelData()[phyObj->GetModelDim() - 1].x * coordScale;
	float y = (float)phyObj->GetCurrModelData()[phyObj->GetModelDim() - 1].y * coordScale;
	float z = 0.0f;

	pastOrbitQuery.push(x);
	pastOrbitQuery.push(y);
	pastOrbitQuery.push(z);
	
	if (pastOrbitQuery.size() > pastOrbitLength * 3)
	{
		pastOrbitQuery.pop();
		pastOrbitQuery.pop();
		pastOrbitQuery.pop();
	}
}
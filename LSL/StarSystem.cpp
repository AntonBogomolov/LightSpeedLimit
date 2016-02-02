#include "StarSystem.h"
#include "Planet.h"
#include "Star.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "ShaderWarper.h"

CStarSystem::CStarSystem()
{
	tmpPhysObjects = new std::vector<CPhysicsObject*>();
	
	twTexturer = new CGLShaderWarper(CShaderManager::getShader(Defines::SH_TEXTURER));
	twTexturer->addTextureDescriptor(STextureDescriptor::TEXD_ATLAS, Defines::ATLAS_STARSYSTEM);

	dnStarSystem = new CSceneGraphNodeDrawableBase("dnStarsystem", NULL, twTexturer, NULL);	
	dnStarSystem->setParams(new CAdditionalParams(CAdditionalParams::DR_PARAM_BLEND));
}

CStarSystem::~CStarSystem()
{
	if (tmpPhysObjects) delete tmpPhysObjects;

	if (twTexturer) delete twTexturer;
	if (dnStarSystem) delete dnStarSystem;
}

const std::vector<CPhysicsObject*>* CStarSystem::getAllPhysicsObjects() const
{	
	tmpPhysObjects->clear();
	tmpPhysObjects->insert(tmpPhysObjects->end(), spaceObjects.begin(), spaceObjects.end());
	return tmpPhysObjects;
}

const std::vector<CSpaceObject*>* CStarSystem::getAllSpaceObjects() const
{
	return &spaceObjects;
}

std::vector<CSpaceObject*>*	CStarSystem::getAllSpaceObjectsForModify()
{
	return &spaceObjects;
}

const std::vector<CStar*>* CStarSystem::getStars() const
{
	return &stars;
}

const std::vector<CPlanet*>* CStarSystem::getPlanets() const
{
	return &planets;
}

CSceneGraphNodeDrawableBase* CStarSystem::getDrawNode()const
{
	return dnStarSystem;
}

void CStarSystem::clearPlanets()
{
	for each (CPlanet* planet in planets)
	{
		removeSpaceObject(planet);
	}
	planets.clear();
}

void CStarSystem::clearStars()
{
	for each (CStar* star in stars)
	{
		removeSpaceObject(star);
	}
	stars.clear();
}

void CStarSystem::clearAll()
{
	spaceObjects.clear();
	planets.clear();
	stars.clear();
}

const double CStarSystem::getDistance(const CSpaceObject* obj1, const CSpaceObject* obj2) const
{
	return glm::distance(obj1->getPos(), obj2->getPos());
}

void CStarSystem::removePlanet(const CPlanet* obj)
{
	if (!obj) return;
	for (auto it = planets.begin(); it != planets.end(); ++it)
	{
		if (obj == (*it))
		{
			planets.erase(it);
			break;
		}
	}
}

void CStarSystem::removeStar(const CStar* obj)
{
	if (!obj) return;
	for (auto it = stars.begin(); it != stars.end(); ++it)
	{
		if (obj == (*it))
		{
			stars.erase(it);
			break;
		}
	}
}

void CStarSystem::removeSpaceObject(const CSpaceObject* obj)
{
	if (!obj) return;
	if (obj->getType() == GameDefines::GAME_TYPE_STAR) removeStar((CStar*)obj);
	if (obj->getType() == GameDefines::GAME_TYPE_PLANET) removePlanet((CPlanet*)obj);
	for (auto it = spaceObjects.begin(); it != spaceObjects.end(); ++it)
	{
		if (obj == (*it))
		{
			spaceObjects.erase(it);
			break;
		}
	}	
}

void CStarSystem::addPlanet(CPlanet* obj)
{
	if (!findSpaceObj(obj))
	{
		addSpaceObject(obj);
		obj->setShaderUpdaterParam(this);
		planets.push_back(obj);

		CStar* currStar = stars.back();
		if (!currStar) return;

		const double dist = getDistance(currStar, obj);
		const double habitableZoneRad = currStar->getHabitableZoneRadius();

		double planetTemp = 20.0 + (habitableZoneRad - dist) * 2.5;
		if (planetTemp < -360.0) planetTemp = -300.0;
		obj->setPlanetTemp((int)planetTemp);		

		obj->calcPlanetGeneratorParams();
	}
}

void CStarSystem::addStar(CStar* obj)
{
	if (!findSpaceObj(obj))
	{
		stars.push_back(obj);
		addSpaceObject(obj);		
	}
}

void CStarSystem::addSpaceObject(CSpaceObject* obj)
{
	spaceObjects.push_back(obj);
	dnStarSystem->AddChild(obj);
	for each (CSpaceObject* satellite in *obj->getSatelliteList())
	{
		if (satellite->getType() == GameDefines::GAME_TYPE_STAR) addStar((CStar*)satellite);
		if (satellite->getType() == GameDefines::GAME_TYPE_PLANET) addPlanet((CPlanet*)satellite);
	}	
}

bool CStarSystem::findSpaceObj(const CSpaceObject* obj)
{
	if (!obj) return false;
	for (auto it = spaceObjects.begin(); it != spaceObjects.end(); ++it)
	{
		if (obj == (*it))
		{
			return true;
		}
	}
	return false;
}
#pragma once

#include <vector>
class CPhysicsObject;
class CSpaceObject;
class CPlanet;
class CStar;

class CGLShaderWarper;
class CSceneGraphNodeDrawableBase;

class CStarSystem
{
private:
	std::vector<CSpaceObject*> spaceObjects;
	std::vector<CStar*> stars;
	std::vector<CPlanet*> planets;

	mutable std::vector<CPhysicsObject*>* tmpPhysObjects;

	CGLShaderWarper* twTexturer;
	CSceneGraphNodeDrawableBase* dnStarSystem;

	void removeSpaceObject(const CSpaceObject* obj);
	void addSpaceObject(CSpaceObject* obj);
	bool findSpaceObj(const CSpaceObject* obj);
public:
	const std::vector<CPhysicsObject*>* getAllPhysicsObjects() const;
	const std::vector<CSpaceObject*>* getAllSpaceObjects() const;
	std::vector<CSpaceObject*>*		  getAllSpaceObjectsForModify();
	const std::vector<CStar*>*		  getStars() const;
	const std::vector<CPlanet*>*	  getPlanets() const;
	const double getDistance(const CSpaceObject* obj1, const CSpaceObject* obj2) const;

	CSceneGraphNodeDrawableBase* getDrawNode()const;

	void clearPlanets();
	void clearStars();
	void clearAll();

	void removePlanet(const CPlanet* obj);
	void removeStar(const CStar* obj);

	void addPlanet(CPlanet* obj);
	void addStar(CStar* obj);

	CStarSystem();
	~CStarSystem();
};
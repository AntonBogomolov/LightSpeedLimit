#pragma once

#include "PhysicsObject.h"
#include "SceneGraphNode.h"
#include "ShaderWarper.h"
#include "GameDefines.h"
#include "EngineDefines.h"
#include "AppDefines.h"

#include <string>
#include <vector>

class CSprite;
class COrbit;
class CGeometricPrimitive;

class CSpaceObject : public CPhysicsObject, public CSceneGraphNodeDrawableBase, public CGameObj, public IShaderUpdater
{	
private:
	std::string name;
	int spriteOffsetX;
	int spriteOffsetY;	
protected:
	CGLShaderWarper* twTexturer;
	CSceneGraphNodeDrawableBase* ndSpaceObjSprite;
	CSceneGraphNodeDrawableBase* ndSpaceObjOrbit;
	CSprite* spaceObjSprite;
	CGeometricPrimitive* futureOrbitGeometry;
	CGeometricPrimitive* pastOrbitGeometry;

	std::vector<CSpaceObject*> satelliteList;	
public:
	void setSpaceObjName(const std::string name);
	const std::string getSpaceObjName()const;

	static const  double screenToSpaceCoord(const int screenCoord);
	static const  int spaceToScreenCoord(const double spaceCoord);
	
	virtual void updatePhysics(const float dt);
	virtual void update(const float dt);
	virtual void updateVBO();

	virtual void setSprite(const int atlasId, const int textureId, const int setsCnt = 1, const int animCnt = 1);
	virtual void setSpriteSize(const int scrW, const int scrH);
	const CSprite* getSprite() const;
		
	const std::vector<CSpaceObject*>* getSatelliteList()const;
	void clearSatelliteList();
	void addSatellite(CSpaceObject* satellite);
	void removeSatellite(const CSpaceObject* satellite);
	const bool isSattelite(const CSpaceObject* satellite)const;
protected:
	virtual void F(const double t, const glm::dvec2* Y, glm::dvec2* FY);
	virtual void updateAllChildPosition(const CSceneGraphNode* node);
	virtual const int  calcSpriteSize();

	virtual void setSpriteOffset(const int xOffset, const int yOffset);
	const int getSpriteOffsetX()const;
	const int getSpriteOffsetY()const;
public:
	CSpaceObject(const double x, const double y, const glm::dvec2 velocity = glm::dvec2(0.0f, 0.0f), const glm::dvec2 acceleration = glm::dvec2(0.0f, 0.0f), const double mass = 1, const double radius = 1);
	virtual ~CSpaceObject();
};


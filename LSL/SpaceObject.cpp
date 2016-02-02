#include "SpaceObject.h"
#include "SceneGraph.h"
#include "AnimatedSprite.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "ShaderWarper.h"
#include "Orbit.h"
#include "GeometricPrimitive.h"

CSpaceObject::CSpaceObject(const double x, const double y, const glm::dvec2 velocity, const glm::dvec2 acceleration, const double mass, const double radius)
	: CPhysicsObject(velocity, acceleration, glm::dvec2(x, y), mass, radius), CSceneGraphNodeDrawableBase("SpaceObj")
	, CGameObj(GameDefines::GAME_TYPE_SPACEOBJ, spaceToScreenCoord(x), spaceToScreenCoord(y))
{
	spaceObjSprite = NULL;
	ndSpaceObjSprite = NULL;
	ndSpaceObjOrbit = NULL;
	twTexturer = NULL;

	spriteOffsetX = 0;
	spriteOffsetY = 0;
	
	getOrbit()->setCoordScale(7.0f);
	futureOrbitGeometry = new CGeometricPrimitive();
	pastOrbitGeometry   = new CGeometricPrimitive();
	setType(GameDefines::GAME_TYPE_SPACEOBJ);

	setHasUpdater(true);
}

CSpaceObject::~CSpaceObject()
{
	satelliteList.clear();

	if (spaceObjSprite)		delete spaceObjSprite;
	if (ndSpaceObjSprite)	delete ndSpaceObjSprite;
	if (ndSpaceObjOrbit)	delete ndSpaceObjOrbit;
	if (twTexturer)			delete twTexturer;

	if (futureOrbitGeometry) delete futureOrbitGeometry;
	if (pastOrbitGeometry)   delete pastOrbitGeometry;
}

void CSpaceObject::updatePhysics(const float dt)
{
	__super::updatePhysics(dt);
}

void CSpaceObject::updateVBO()
{
	spaceObjSprite->updateManuallyVBO();

	if (getOrbit()->getIsNeedToCalcOrbit())
	{
		futureOrbitGeometry->setGeomDataPointer(getOrbit()->getFutureOrbitData(), getOrbit()->getFutureOrbitDataLength());
		futureOrbitGeometry->updateManuallyVBO();
		pastOrbitGeometry->setGeomDataPointer(getOrbit()->getPastOrbitData(), getOrbit()->getPastOrbitDataLength());
		pastOrbitGeometry->updateManuallyVBO();
	}
}

void CSpaceObject::updateAllChildPosition(const CSceneGraphNode* node)
{
	if (isLocked()) return;

	int x = spaceToScreenCoord(getPos().x) + spriteOffsetX;
	int y = spaceToScreenCoord(getPos().y) + spriteOffsetY;
	setCenterAbsoluteCoords(x, y);

	for each (CSpaceObject* satellite in satelliteList)
	{		
		glm::dvec2 satVec = this->getPos() - satellite->getPos();
		int objRad = (int)(calcSpriteSize() / 2.0 * 1.2);
		if (spaceToScreenCoord(glm::length(satVec)) < objRad)
		{
			satVec = glm::normalize(satVec);
			satVec *= objRad;
			satellite->setSpriteOffset((int)satVec.x,(int)(satVec.y * 1.0));
		}
	}
	
	for each (CSceneGraphNode* child in *node->GetChilds())
	{
		if (child->getNodeType() == CSceneGraph::GR_NODE_TYPE_DRAWABLE)
		{
			CSceneGraphNodeDrawable* drawChild = (CSceneGraphNodeDrawable*)child;
			drawChild->setCenterAbsoluteCoords(x, y);		
		}
		else
		{
			updateAllChildPosition(child);
		}
	}	
}

void CSpaceObject::F(const double t, const glm::dvec2* Y, glm::dvec2* FY)
{
	FY[0] = getAcceleration();
	FY[1] = Y[0];
}

void CSpaceObject::setSpaceObjName(const std::string name)
{
	this->name = name;
}

const std::string CSpaceObject::getSpaceObjName()const
{
	return name;
}

const double CSpaceObject::screenToSpaceCoord(const int screenCoord)
{
	return screenCoord / 7.0;
}

const int CSpaceObject::spaceToScreenCoord(const double spaceCoord)
{
	return (int)(spaceCoord * 7);
}

const int CSpaceObject::getSpriteOffsetX()const
{
	return spriteOffsetX;
}

const int CSpaceObject::getSpriteOffsetY()const
{
	return spriteOffsetY;
}

void CSpaceObject::setSprite(const int atlasId, const int textureId, const int setsCnt, const int animCnt)
{
	if (!twTexturer)
	{
		twTexturer = new CGLShaderWarper(CShaderManager::getShader(Defines::SH_TEXTURER), STextureDescriptor::TEXD_ATLAS, Defines::ATLAS_STARSYSTEM);		
		twTexturer->setShaderUpdater(this);
	}
	if (!ndSpaceObjSprite)
	{
		ndSpaceObjSprite = new CSceneGraphNodeDrawableBase(getSpaceObjName(), NULL, twTexturer, new CAdditionalParams(CAdditionalParams::DR_PARAM_BLEND));
	}
	if (!ndSpaceObjOrbit)
	{
		CGLShaderWarper* orbitShaderWarper = new CGLShaderWarper(CShaderManager::getShader(Defines::SH_ORBITS));
		orbitShaderWarper->setShaderUpdater(this);
		ndSpaceObjOrbit = new CSceneGraphNodeDrawableBase(getSpaceObjName(), NULL, orbitShaderWarper, new CAdditionalParams(CAdditionalParams::DR_PARAM_BLEND));	
	}
	
	int spriteSize = calcSpriteSize();

	if (!spaceObjSprite)
	{
		if (setsCnt >= 1 || animCnt >= 1) spaceObjSprite = new CAnimatedSprite(atlasId,textureId, spriteSize, spriteSize, setsCnt, animCnt);
		else spaceObjSprite = new CSprite(atlasId, textureId, spriteSize, spriteSize);
		
		ndSpaceObjSprite->AddChild(spaceObjSprite);
		if (getOrbit()->getIsNeedToCalcOrbit())
		{
			ndSpaceObjOrbit->AddChild(futureOrbitGeometry);
			ndSpaceObjOrbit->AddChild(pastOrbitGeometry);
		}		
		AddChild(ndSpaceObjOrbit);
		AddChild(ndSpaceObjSprite);
	}

	spaceObjSprite->setCenterAbsoluteCoords(spaceToScreenCoord(getPos().x), spaceToScreenCoord(getPos().y));
	spaceObjSprite->setWidth(spriteSize);
	spaceObjSprite->setHeight(spriteSize);		
}

void CSpaceObject::setSpriteSize(const int scrW, const int scrH)
{
	if (!spaceObjSprite) return;

	spaceObjSprite->setWidth(scrW);
	spaceObjSprite->setHeight(scrH);
}

void CSpaceObject::setSpriteOffset(const int xOffset, const int yOffset)
{
	spriteOffsetX = xOffset;
	spriteOffsetY = yOffset;
}

const CSprite* CSpaceObject::getSprite() const
{
	return spaceObjSprite;
}

const int CSpaceObject::calcSpriteSize()
{
	return EngineSettings::spriteDefaultSize;
}

void CSpaceObject::update(const float dt)
{
	updateAllChildPosition(this);
}

const std::vector<CSpaceObject*>* CSpaceObject::getSatelliteList()const
{
	return &satelliteList;
}

void CSpaceObject::clearSatelliteList()
{
	satelliteList.clear();
}

void CSpaceObject::addSatellite(CSpaceObject* satellite)
{
	if (isSattelite(satellite)) return;

	satellite->setCircularOrbit(this);
	satelliteList.push_back(satellite);	
}

void CSpaceObject::removeSatellite(const CSpaceObject* satellite)
{
	if (!satellite) return;
	for (auto it = satelliteList.begin(); it != satelliteList.end(); ++it)
	{
		if (satellite == (*it))
		{
			satelliteList.erase(it);
			break;
		}
	}
}

const bool CSpaceObject::isSattelite(const CSpaceObject* satellite)const
{
	if (!satellite) return false;
	for (auto it = satelliteList.begin(); it != satelliteList.end(); ++it)
	{
		if (satellite == (*it))
		{
			return true;
		}
	}

	return false;
}
#pragma once
#include "SceneGraphNode.h"
class CGeometricPrimitive :
	public CSceneGraphNodeDrawable
{
private:
	bool needToUpdateVBO;
	bool isUsedExternalGeomData;

	float* geomData;
	const float* constGeomData;
	int geomDataLength;

private:
	void clearGeomData();

public:
	const bool isNeedToUpdateVBO()const;
	void isNeedToUpdateVBO(const bool need);
	
	virtual void update(const float dt);
	void updateManuallyVBO();

	void setGeomDataPointer(const float* data, const int dataLength);
	void setGeomData(float* data, const int dataLength);
	
	CGeometricPrimitive();
	virtual ~CGeometricPrimitive();
};


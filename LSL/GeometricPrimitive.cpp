#include "GeometricPrimitive.h"
#include "VertexBuffer.h"
#include "GLUtils.h"
#include "global.h"

CGeometricPrimitive::CGeometricPrimitive() : CSceneGraphNodeDrawable()
{
	VBO->setMode(GL_LINES);	

	needToUpdateVBO = true;
	setType(CGameObj::TYPE_GEOM_PRIMITIVE);

	isUsedExternalGeomData = false;

	geomData = NULL;
	constGeomData = NULL;
	geomDataLength = 0;
}

CGeometricPrimitive::~CGeometricPrimitive()
{
	clearGeomData();
}

void CGeometricPrimitive::clearGeomData()
{
	if (!isUsedExternalGeomData && geomData != NULL) delete[] geomData;

	geomData = NULL;
	constGeomData = NULL;
	geomDataLength = 0;
}

const bool CGeometricPrimitive::isNeedToUpdateVBO()const
{
	return needToUpdateVBO;
}

void CGeometricPrimitive::isNeedToUpdateVBO(const bool need)
{
	needToUpdateVBO = need;
}

void CGeometricPrimitive::update(const float dt)
{

}

void CGeometricPrimitive::updateManuallyVBO()
{
	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO) return;
	
	const int indexCnt = (geomDataLength / 3) * 2;

	short* ind = new short[indexCnt];
	for (int i = 0; i < indexCnt / 2; i++)
	{
		ind[i * 2 + 0] = i;
		ind[i * 2 + 1] = i + 1;
	}
	ind[indexCnt - 2] = indexCnt - 2;
	ind[indexCnt - 1] = indexCnt - 2;

	if (isUsedExternalGeomData) VBO->setVerticesData(constGeomData, geomDataLength);
	else						VBO->setVerticesData(geomData,		geomDataLength);
	VBO->setIndicesData(ind, indexCnt);

	delete[] ind;
}

void CGeometricPrimitive::setGeomDataPointer(const float* data, const int dataLength)
{
	if (data == NULL || dataLength == 0) return;
	clearGeomData();

	constGeomData = data;
	geomDataLength = dataLength;

	isUsedExternalGeomData = true;
}

void CGeometricPrimitive::setGeomData(float* data, const int dataLength)
{
	if (data == NULL || dataLength == 0) return;
	clearGeomData();

	geomData = data;
	geomDataLength = dataLength;

	isUsedExternalGeomData = false;
}
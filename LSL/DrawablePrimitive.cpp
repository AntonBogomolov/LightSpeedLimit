#include "DrawablePrimitive.h"
#include "VideoManeger.h"
#include "VertexBuffer.h"

CDrawablePrimitive::CDrawablePrimitive(int x, int y, int width, int height) : CSceneGraphNodeDrawable()
{
	this->x = x;
	this->y = y;
	this->width  = width;
	this->height = height;

	updateManuallyVBO();
}

CDrawablePrimitive::~CDrawablePrimitive()
{

}

void CDrawablePrimitive::update(const float dt)
{

}

void CDrawablePrimitive::updateManuallyVBO()
{
	CVertexBuffer* VBO = getVBOForModify();
	if (!VBO) return;

	GLfloat qverts[] = { x, y, 0, x + width, y, 0, x + width, y + height, 0, x, y + height, 0 };
	GLfloat qcoords[] = { 0, 1, 1, 1, 1, 0, 0, 0 };
	short qind[] = { 0, 1, 2, 0, 2, 3 };

	VBO->setVerticesData(qverts, 4 * 3);
	VBO->setTexData(qcoords, 4 * 2);
	VBO->setIndicesData(qind, 6);
}
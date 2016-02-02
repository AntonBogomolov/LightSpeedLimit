#include "global.h"
#include "GLUtils.h"
#include "GLShaderObject.h"
#include "TextureManager.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "SceneGraphNode.h"
#include "ScreenObj.h"
#include "VideoManeger.h"
#include "DrawablePrimitive.h"

CGLUtils* CGLUtils::instance = NULL;

CVertexBuffer* CGLUtils::quad		= NULL;
CVertexBuffer* CGLUtils::revQuad	= NULL;
CVertexBuffer* CGLUtils::lQuad		= NULL;

CDrawablePrimitive* CGLUtils::QuadNode = NULL;
CDrawablePrimitive* CGLUtils::revQuadNode = NULL;

byte* CGLUtils::glGetTexImage(const int texId, const int texW, const int texH)
{
	GLuint* framebuffer = new GLuint[1];
	glGenFramebuffers(1, framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer[0]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, CTextureManager::getTexture(texId)->getID(), 0); // texId
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		//	Log.e("glGetTexImage", "image load faild");
	}
	byte* buff = new byte[texW * texH * 4];

	glReadPixels(0, 0, texW, texH, GL_RGBA, GL_UNSIGNED_BYTE, buff);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, framebuffer);

	return buff;
}

void CGLUtils::checkGlError(const string op)
{
	int error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
	
	}
}

CSceneGraphNodeDrawable* CGLUtils::getFullscreenDrawableNode()
{
	if (!QuadNode) initFullscreenQuad();
	return (CSceneGraphNodeDrawable*)QuadNode;
}
CSceneGraphNodeDrawable* CGLUtils::getFullscreenRevDrawableNode()
{
	if (!revQuadNode) initFullscreenRevQuad();
	return (CSceneGraphNodeDrawable*)revQuadNode;
}

CVertexBuffer* CGLUtils::initFullscreenQuad()
{

	if (quad) delete quad;
	quad = NULL;
	quad = new CVertexBuffer();	
	if (!QuadNode) QuadNode = new CDrawablePrimitive();
	
	GLfloat qverts[] = { 0, 0, 0, CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight, 0 };
	GLfloat qcoords[] = { 0, 0, 1, 0, 1, 1, 0, 1 };
	short qind[] = { 0, 1, 2, 0, 2, 3 };

	quad->setVerticesData(qverts, 4 * 3);
	quad->setTexData(qcoords, 4 * 2);
	quad->setIndicesData(qind, 6);

	QuadNode->setVBO(quad);

	return quad;
}

void CGLUtils::drawFullscreenQuad(const CGLShaderObject* currShader)
{
	if (quad == NULL)  initFullscreenQuad();
	if (quad)
	{
		quad->drawElements(0, currShader);
	}
}

CVertexBuffer* CGLUtils::initFullscreenRevQuad()
{
	if (revQuad) delete revQuad;
	revQuad = NULL;
	revQuad = new CVertexBuffer();
	if (!revQuadNode) revQuadNode = new CDrawablePrimitive();
		
	GLfloat qverts[] = { 0, 0, 0, CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight, 0 };
	GLfloat qcoords[] = { 0, 1, 1, 1, 1, 0, 0, 0 };
	short qind[] = { 0, 1, 2, 0, 2, 3 };

	revQuad->setVerticesData(qverts, 4 * 3);
	revQuad->setTexData(qcoords, 4 * 2);
	revQuad->setIndicesData(qind, 6);

	revQuadNode->setVBO(revQuad);
	return revQuad;
}

void CGLUtils::drawFullscreenRevQuad(const CGLShaderObject* currShader)
{
	if (revQuad == NULL)  initFullscreenRevQuad();
	if (revQuad)
	{
		revQuad->drawElements(0, currShader);
	}
}

CVertexBuffer* CGLUtils::initLFullscreenQuad()
{
	float flDataVert[] = {	0, 0, 0, 0, 0,
							CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, 0,
							0, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight,
							CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, 0,
							0, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight,
							CVideoManeger::scrWidth, CVideoManeger::scrHeight, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight
	};

	return NULL;
}

void CGLUtils::drawLFullscreenQuad(const CGLShaderObject* currShader)
{
	if (lQuad == NULL)  initLFullscreenQuad();	
}

void CGLUtils::fillVBOData( float* vert, float* tex, float* vtex, short* ind, const CTexCoord* sprite, const CTexCoord* vsprite,
							const int index, const CScreenObj* obj, const int direction, const float offset)
{
	bool hasAdditionalTexCoord = false;
	if (vtex != NULL && vsprite != NULL) hasAdditionalTexCoord = true;

	if (vert == NULL)  return;
	if (tex == NULL )  return;
	if (ind == NULL )  return;
	if (sprite == NULL) return;
		
	const glm::vec2* points = obj->getPointsCoords();
	int X1 = points[0].x;
	int Y1 = points[0].y;
	int X2 = points[1].x;
	int Y2 = points[1].y;
	int X3 = points[2].x;
	int Y3 = points[2].y;
	int X4 = points[3].x;
	int Y4 = points[3].y;

	vert[index * 12 + 0] = X1;				vert[index * 12 + 1] = Y1;				vert[index * 12 + 2] = 0;
	vert[index * 12 + 3] = X2 + offset;		vert[index * 12 + 4] = Y2;				vert[index * 12 + 5] = 0;
	vert[index * 12 + 6] = X3 + offset;		vert[index * 12 + 7] = Y3 + offset;		vert[index * 12 + 8] = 0;
	vert[index * 12 + 9] = X4;				vert[index * 12 + 10] =Y4 + offset;		vert[index * 12 + 11] = 0;
	
	switch (direction)
	{
	case Defines::DIR_DOWN:
		tex[index * 8 + 0] = sprite->tx + sprite->twidth;		tex[index * 8 + 1] = sprite->ty + sprite->theight;
		tex[index * 8 + 2] = sprite->tx;						tex[index * 8 + 3] = sprite->ty + sprite->theight;
		tex[index * 8 + 4] = sprite->tx;						tex[index * 8 + 5] = sprite->ty;
		tex[index * 8 + 6] = sprite->tx + sprite->twidth;		tex[index * 8 + 7] = sprite->ty;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 1] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 2] = vsprite->tx;						vtex[index * 8 + 3] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 4] = vsprite->tx;						vtex[index * 8 + 5] = vsprite->ty;
			vtex[index * 8 + 6] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 7] = vsprite->ty;
		}
		break;
	case Defines::DIR_LEFT:
		tex[index * 8 + 0] = sprite->tx + sprite->twidth;		tex[index * 8 + 1] = sprite->ty;
		tex[index * 8 + 2] = sprite->tx + sprite->twidth;		tex[index * 8 + 3] = sprite->ty + sprite->theight;
		tex[index * 8 + 4] = sprite->tx;						tex[index * 8 + 5] = sprite->ty + sprite->theight;
		tex[index * 8 + 6] = sprite->tx;						tex[index * 8 + 7] = sprite->ty;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 1] = vsprite->ty;
			vtex[index * 8 + 2] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 3] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 4] = vsprite->tx;						vtex[index * 8 + 5] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 6] = vsprite->tx;						vtex[index * 8 + 7] = vsprite->ty;
		}
		break;
	case Defines::DIR_RIGHT:
		tex[index * 8 + 0] = sprite->tx;						tex[index * 8 + 1] = sprite->ty + sprite->theight;
		tex[index * 8 + 2] = sprite->tx;						tex[index * 8 + 3] = sprite->ty;
		tex[index * 8 + 4] = sprite->tx + sprite->twidth;		tex[index * 8 + 5] = sprite->ty;
		tex[index * 8 + 6] = sprite->tx + sprite->twidth;		tex[index * 8 + 7] = sprite->ty + sprite->theight;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx;						vtex[index * 8 + 1] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 2] = vsprite->tx;						vtex[index * 8 + 3] = vsprite->ty;
			vtex[index * 8 + 4] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 5] = vsprite->ty;
			vtex[index * 8 + 6] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 7] = vsprite->ty + vsprite->theight;
		}
		break;

	default:
		tex[index * 8 + 0] = sprite->tx;						tex[index * 8 + 1] = sprite->ty;
		tex[index * 8 + 2] = sprite->tx + sprite->twidth;		tex[index * 8 + 3] = sprite->ty;
		tex[index * 8 + 4] = sprite->tx + sprite->twidth;		tex[index * 8 + 5] = sprite->ty + sprite->theight;
		tex[index * 8 + 6] = sprite->tx;						tex[index * 8 + 7] = sprite->ty + sprite->theight;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx;						vtex[index * 8 + 1] = vsprite->ty;
			vtex[index * 8 + 2] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 3] = vsprite->ty;
			vtex[index * 8 + 4] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 5] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 6] = vsprite->tx;						vtex[index * 8 + 7] = vsprite->ty + vsprite->theight;
		}
		break;
	}

	ind[index * 6 + 0] = (short)(index * 4);	ind[index * 6 + 1] = (short)(index * 4 + 1);	ind[index * 6 + 2] = (short)(index * 4 + 2);
	ind[index * 6 + 3] = (short)(index * 4);	ind[index * 6 + 4] = (short)(index * 4 + 2);	ind[index * 6 + 5] = (short)(index * 4 + 3);
}

void CGLUtils::fillVBOVertData(float* vert, const int index, const CScreenObj* obj, const float offset)
{
	if (vert == NULL)  return;
	
	const glm::vec2* points = obj->getPointsCoords();
	int X1 = points[0].x;
	int Y1 = points[0].y;
	int X2 = points[1].x;
	int Y2 = points[1].y;
	int X3 = points[2].x;
	int Y3 = points[2].y;
	int X4 = points[3].x;
	int Y4 = points[3].y;

	vert[index * 12 + 0] = X1;				vert[index * 12 + 1] = Y1;				vert[index * 12 + 2] = 0;
	vert[index * 12 + 3] = X2 + offset;		vert[index * 12 + 4] = Y2;				vert[index * 12 + 5] = 0;
	vert[index * 12 + 6] = X3 + offset;		vert[index * 12 + 7] = Y3 + offset;		vert[index * 12 + 8] = 0;
	vert[index * 12 + 9] = X4;				vert[index * 12 + 10] =Y4 + offset;		vert[index * 12 + 11] = 0;
}

void CGLUtils::fillVBOTexData(float* tex, float* vtex, const CTexCoord* sprite, const CTexCoord* vsprite, const int index, const int direction, const float offset)
{
	bool hasAdditionalTexCoord = false;
	if (vtex != NULL && vsprite != NULL) hasAdditionalTexCoord = true;
	
	if (tex == NULL)  return;
	if (sprite == NULL) return;
	
	switch (direction)
	{
	case Defines::DIR_DOWN:
		tex[index * 8 + 0] = sprite->tx + sprite->twidth;		tex[index * 8 + 1] = sprite->ty + sprite->theight;
		tex[index * 8 + 2] = sprite->tx;						tex[index * 8 + 3] = sprite->ty + sprite->theight;
		tex[index * 8 + 4] = sprite->tx;						tex[index * 8 + 5] = sprite->ty;
		tex[index * 8 + 6] = sprite->tx + sprite->twidth;		tex[index * 8 + 7] = sprite->ty;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 1] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 2] = vsprite->tx;						vtex[index * 8 + 3] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 4] = vsprite->tx;						vtex[index * 8 + 5] = vsprite->ty;
			vtex[index * 8 + 6] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 7] = vsprite->ty;
		}
		break;
	case Defines::DIR_LEFT:
		tex[index * 8 + 0] = sprite->tx + sprite->twidth;		tex[index * 8 + 1] = sprite->ty;
		tex[index * 8 + 2] = sprite->tx + sprite->twidth;		tex[index * 8 + 3] = sprite->ty + sprite->theight;
		tex[index * 8 + 4] = sprite->tx;						tex[index * 8 + 5] = sprite->ty + sprite->theight;
		tex[index * 8 + 6] = sprite->tx;						tex[index * 8 + 7] = sprite->ty;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 1] = vsprite->ty;
			vtex[index * 8 + 2] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 3] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 4] = vsprite->tx;						vtex[index * 8 + 5] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 6] = vsprite->tx;						vtex[index * 8 + 7] = vsprite->ty;
		}
		break;
	case Defines::DIR_RIGHT:
		tex[index * 8 + 0] = sprite->tx;						tex[index * 8 + 1] = sprite->ty + sprite->theight;
		tex[index * 8 + 2] = sprite->tx;						tex[index * 8 + 3] = sprite->ty;
		tex[index * 8 + 4] = sprite->tx + sprite->twidth;		tex[index * 8 + 5] = sprite->ty;
		tex[index * 8 + 6] = sprite->tx + sprite->twidth;		tex[index * 8 + 7] = sprite->ty + sprite->theight;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx;						vtex[index * 8 + 1] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 2] = vsprite->tx;						vtex[index * 8 + 3] = vsprite->ty;
			vtex[index * 8 + 4] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 5] = vsprite->ty;
			vtex[index * 8 + 6] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 7] = vsprite->ty + vsprite->theight;
		}
		break;

	default:
		tex[index * 8 + 0] = sprite->tx;						tex[index * 8 + 1] = sprite->ty;
		tex[index * 8 + 2] = sprite->tx + sprite->twidth;		tex[index * 8 + 3] = sprite->ty;
		tex[index * 8 + 4] = sprite->tx + sprite->twidth;		tex[index * 8 + 5] = sprite->ty + sprite->theight;
		tex[index * 8 + 6] = sprite->tx;						tex[index * 8 + 7] = sprite->ty + sprite->theight;

		if (hasAdditionalTexCoord)
		{
			vtex[index * 8 + 0] = vsprite->tx;						vtex[index * 8 + 1] = vsprite->ty;
			vtex[index * 8 + 2] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 3] = vsprite->ty;
			vtex[index * 8 + 4] = vsprite->tx + vsprite->twidth;	vtex[index * 8 + 5] = vsprite->ty + vsprite->theight;
			vtex[index * 8 + 6] = vsprite->tx;						vtex[index * 8 + 7] = vsprite->ty + vsprite->theight;
		}
		break;
	}
}

void CGLUtils::fillVBOIndData(short* ind, const int index)
{	
	if (ind == NULL)  return;

	ind[index * 6 + 0] = (short)(index * 4);	ind[index * 6 + 1] = (short)(index * 4 + 1);	ind[index * 6 + 2] = (short)(index * 4 + 2);
	ind[index * 6 + 3] = (short)(index * 4);	ind[index * 6 + 4] = (short)(index * 4 + 2);	ind[index * 6 + 5] = (short)(index * 4 + 3);
}

void CGLUtils::fillVBOData(float* vert, short* ind, const int index, const float* geomData, const int geomDataCoordCnt, const float offset)
{
	if (vert == NULL || ind == NULL || geomData == NULL)  return;
	if (geomDataCoordCnt < 1 || geomDataCoordCnt > 3) return;

	int X = (int)geomData[index * geomDataCoordCnt + 0];
	int Y = (int)geomData[index * geomDataCoordCnt + 1];
	int Z = (int)geomData[index * geomDataCoordCnt + 2];

	if (geomDataCoordCnt == 1)
	{
		X = (int)geomData[index * geomDataCoordCnt + 0];
		Y = 0;
		Z = 0;
	}
	if (geomDataCoordCnt == 2)
	{
		X = (int)geomData[index * geomDataCoordCnt + 0];
		Y = (int)geomData[index * geomDataCoordCnt + 1];
		Z = 0;
	}

	vert[index * 3 + 0] = X;	vert[index * 3 + 1] = Y;	vert[index * 3 + 2] = Z;	
	ind[index] = index;
}

CGLUtils::CGLUtils()
{
}

CGLUtils::~CGLUtils()
{
	if (quad) delete quad;
	if (lQuad) delete lQuad;
	if (revQuad) delete revQuad;

	if (QuadNode)	 delete QuadNode;
	if (revQuadNode) delete revQuadNode;
}


CGLUtils* CGLUtils::GetInstance()
{
	if (instance == NULL)
	{
		instance = new CGLUtils();
	}
	return instance;
}

void CGLUtils::resizeWindow()
{
	glViewport(0, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight);

	if (QuadNode)
	{
		GLfloat qverts[] = { 0, 0, 0, CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight, 0 };
		QuadNode->getVBOForModify()->setVerticesData(qverts, 4 * 3);
	}
	if (revQuadNode)
	{
		GLfloat qverts[] = { 0, 0, 0, CVideoManeger::scrWidth, 0, 0, CVideoManeger::scrWidth, CVideoManeger::scrHeight, 0, 0, CVideoManeger::scrHeight, 0 };
		revQuadNode->getVBOForModify()->setVerticesData(qverts, 4 * 3);
	}
}
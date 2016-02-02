#pragma once

class CVertexArray;
class CVertexBuffer;
class CTexCoord;
class CGLShaderObject;
class CSceneGraphNodeDrawable;
class CScreenObj;
class CDrawablePrimitive;

#define byte unsigned char

class CGLUtils
{
private:
	static CGLUtils* instance;
	CGLUtils(void);

	static CVertexBuffer* quad;
	static CVertexBuffer* revQuad;
	static CVertexBuffer* lQuad;	

	static CDrawablePrimitive* revQuadNode;
	static CDrawablePrimitive* QuadNode;
public:

	static void fillVBOData(float* vert, float* tex, float* vtex, short* ind, const CTexCoord* sprite, const CTexCoord* vsprite, const int index, const CScreenObj* obj, const int direction, const float offset);
	static void fillVBOVertData(float* vert, const int index, const CScreenObj* obj, const float offset);
	static void fillVBOTexData(float* tex, float* vtex, const CTexCoord* sprite, const CTexCoord* vsprite, const int index, const int direction, const float offset);
	static void fillVBOIndData(short* ind, const int index);
	static void fillVBOData(float* vert, short* ind, const int index, const float* geomData, const int geomDataCoordCnt, const float offset);
	
	static void drawFullscreenRevQuad(const CGLShaderObject* currShader);
	static void drawLFullscreenQuad(const CGLShaderObject* currShader);
	static void drawFullscreenQuad(const CGLShaderObject* currShader);
	static CSceneGraphNodeDrawable* getFullscreenDrawableNode();
	static CSceneGraphNodeDrawable* getFullscreenRevDrawableNode();

	static CVertexBuffer* initFullscreenRevQuad();
	static CVertexBuffer* initLFullscreenQuad();
	static CVertexBuffer* initFullscreenQuad();

	static void resizeWindow();
	
	static void checkGlError(const std::string op);

	static byte* glGetTexImage(const int texId, const int texW, const int texH);
	
	static CGLUtils* GetInstance();
	void Init();

	~CGLUtils();
};


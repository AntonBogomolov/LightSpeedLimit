#pragma once
#include "IScene.h"
#include <vector>
#include <map>

#include "../../tools/includes/glm/glm.hpp"
#include "EngineDefines.h"

class CFrameBuffer;
class CVertexBuffer;
class CGLShaderObject;
class CGLShaderWarper;
class CSceneGraphNode;
class CSceneGraphNodeDrawable;
class CGLMatrixSet;
class CDrawableModifier;
class CAdditionalParams;
class CGameObj;

class CScene : public IScene
{
protected:
	CDrawableModifier* drawableModifier;
	std::vector<CSceneGraphNode*>* nodesListHelperForDraw;
	std::vector<CSceneGraphNode*>* nodesListHelperForUpdate;
	CGLMatrixSet* glMatrix;
	CSceneGraphNode* rootNode;
	bool needToInit;

	virtual void clearGameObjects();
	virtual void initGameObjects();
private:
	void drawNode(	const CSceneGraphNodeDrawable* drawableNode, const glm::mat4* modelMatrix, const CGLShaderWarper* shader = NULL,
					const CFrameBuffer* FBO = NULL, const CAdditionalParams* additionalParams = NULL);
public:
	virtual void draw();

	virtual void clearScene();
	virtual void initScene();
	virtual void updateScene(const float dt);
	virtual void resizeScene();
	virtual void pauseScene();
	virtual void resumeScene();
	
	const CSceneGraphNode* getRootNode()const;
	
	CScene();
	virtual ~CScene();
};

class CAdditionalParams
{
public:
	static const int DR_PARAM_NONE  = 0;
	static const int DR_PARAM_BLEND = 1;
	static const int DR_PARAM_LOCKTOSCREEN = 2;
public:
	int paramID;
	int intArg1;
	int intArg2;
	int intArg3;
	float flArg1;
	float flArg2;
	float flArg3;

	CAdditionalParams(int paramID = DR_PARAM_NONE);
};

class CDrawableModifier
{
private:
	std::map<int, const CGLShaderWarper*>* modShader;
	std::map<int, const CFrameBuffer*>* modFBO;
	std::map<int, const CAdditionalParams*>* modParams;
public:
	const bool isEmpty();
	void clearData();

	void freeModifiers(const int nodeID);

	void addData(const int nodeID, const glm::mat4 modMatrix = glm::mat4(1.0f), const CGLShaderWarper* modShader = NULL,
				 const CFrameBuffer* modeFBO = NULL, const CAdditionalParams* additionalParams = NULL);

	const CGLShaderWarper* getLastShader()const;
	const CGLShaderWarper* popLastShader();
	const CFrameBuffer* getLastFBO()const;
	const CFrameBuffer* popLastFBO();
	const CAdditionalParams* getLastParams()const;
	const CAdditionalParams* popLastParams();

	CDrawableModifier();
	~CDrawableModifier();
};
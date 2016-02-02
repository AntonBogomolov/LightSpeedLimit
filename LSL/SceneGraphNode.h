#pragma once

#include "GameObj.h"

#include <vector>
#include <string>
#include "../../tools/includes/glm/glm.hpp"

class CFrameBuffer;
class CVertexBuffer;
class CGLShaderObject;
class CGLShaderWarper;
class CAdditionalParams;

class CSceneGraphNode
{
protected:	
	int nodeType;
	std::vector<CSceneGraphNode*> *childs;
	CSceneGraphNode** childsRawData;
	std::string nodeName;	
	bool hasUpdater;
public:
	static const int MAX_CHILDS_PER_NODE = 10000;	
protected:
	void updateChildsRawData();
public:
	const int getNodeType() const;
	const std::string getNodeName()const;
	void setNodeName(const std::string name);

	void setHasUpdater(const bool hasUpdater);
	bool getHasUpdater() const;

	const std::vector<CSceneGraphNode*>* GetChilds()const;
	void AddChild(CSceneGraphNode* childNode);
	const int getChildsCnt()const;
	CSceneGraphNode** getChildsRawData();

	virtual const bool doWork() = 0;

	CSceneGraphNode(const int nodeType, const std::string name = "");
	virtual ~CSceneGraphNode();
};

class CSceneGraphNodeGroup :public CSceneGraphNode
{
public :
	virtual const bool doWork();

	CSceneGraphNodeGroup(const std::string name = "");
	virtual ~CSceneGraphNodeGroup();
};

class CSceneGraphNodeTransform :public CSceneGraphNode
{
private:
	glm::mat4 *transformMatrix;
public:
	virtual const bool doWork();

	const glm::mat4* getTansformMatrix()const;
	void setTansformMatrix(const glm::mat4* transformMatrix);

	CSceneGraphNodeTransform(const std::string name = "", const glm::mat4* transformMatrix = NULL);
	virtual ~CSceneGraphNodeTransform();
};

class CSceneGraphNodeDrawableBase :public CSceneGraphNode
{
protected:
	CFrameBuffer* FBO;
	CGLShaderWarper* shader;	
	CAdditionalParams* params;
public:
	void setFBO(CFrameBuffer* FBO);
	const CFrameBuffer* getFBO()const;
	void setShaderWarper(CGLShaderWarper* shader);
	const CGLShaderWarper* getShaderWarper()const;
	void setParams(CAdditionalParams* params);
	const CAdditionalParams* getParams()const;

	virtual const bool doWork();

	CSceneGraphNodeDrawableBase(const std::string name = "", CFrameBuffer* FBO = NULL, CGLShaderWarper* shader = NULL, CAdditionalParams* params = NULL);
	virtual ~CSceneGraphNodeDrawableBase();
};

class CSceneGraphNodeDrawable :public CSceneGraphNode, public CGameObj
{
protected:
	CVertexBuffer *VBO;
	bool onScreen;
public:
	virtual const bool doWork();
	//virtual void update(const float dt);
	
	const bool isOnScreen()const;
	const CVertexBuffer* getVBO()const;
	CVertexBuffer* getVBOForModify()const;
	void setVBO(CVertexBuffer* VBO);
	virtual void updateManuallyVBO() = 0;

	CSceneGraphNodeDrawable(const std::string name = "", CVertexBuffer *VBO = NULL);
	virtual ~CSceneGraphNodeDrawable();
};
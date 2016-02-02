#include "SceneGraphNode.h"
#include "SceneGraph.h"

#include "global.h"
#include "FrameBuffer.h"
#include "VertexBuffer.h"
#include "GLShaderObject.h"
#include "EngineDefines.h"
#include "Scene.h"

CSceneGraphNode::CSceneGraphNode(const int nodeType, const std::string name)
{
	this->nodeType = nodeType;
	nodeName = name;
	childs = new std::vector<CSceneGraphNode*>();

	hasUpdater = false;
}

CSceneGraphNode::~CSceneGraphNode()
{
	if (childs)
	{
		childs->clear();
	}
	delete childs;	
	if (childsRawData) delete[] childsRawData;
}

const int CSceneGraphNode::getNodeType()const
{
	return nodeType;
}

const std::vector<CSceneGraphNode*>* CSceneGraphNode::GetChilds()const
{
	return childs;
}

void CSceneGraphNode::AddChild(CSceneGraphNode* childNode)
{
	childs->push_back(childNode);
}

CSceneGraphNode** CSceneGraphNode::getChildsRawData()
{
	if (!childsRawData) updateChildsRawData();
	return childsRawData;
}

void CSceneGraphNode::updateChildsRawData()
{
	if (childsRawData) delete[] childsRawData;
	childsRawData = new CSceneGraphNode*[MAX_CHILDS_PER_NODE];
	int i = 0;
	for (auto it = childs->rbegin(); it != childs->rend(); it++)
	{
		childsRawData[i] = *it;
		i++;
	}
}

const int CSceneGraphNode::getChildsCnt()const
{
	return childs->size();
}

const std::string CSceneGraphNode::getNodeName()const
{
	return nodeName;
}

void CSceneGraphNode::setNodeName(const std::string name)
{
	nodeName = name;
}

void CSceneGraphNode::setHasUpdater(const bool hasUpdater)
{
	this->hasUpdater = hasUpdater;
}

bool CSceneGraphNode::getHasUpdater() const
{
	return hasUpdater;
}

/////////////////////////////////////////////////////////////////////

const bool CSceneGraphNodeGroup::doWork()
{
	return true;
}

CSceneGraphNodeGroup::CSceneGraphNodeGroup(const std::string name) : CSceneGraphNode(CSceneGraph::GR_NODE_TYPE_GROUP, name)
{

}

CSceneGraphNodeGroup::~CSceneGraphNodeGroup()
{

}


/////////////////////////////////////////////////////////////////////

const bool CSceneGraphNodeTransform::doWork()
{
	return true;
}

CSceneGraphNodeTransform::CSceneGraphNodeTransform(const std::string name, const glm::mat4* transformMatrix) : CSceneGraphNode(CSceneGraph::GR_NODE_TYPE_TRANSFORM, name)
{
	if (!transformMatrix) this->transformMatrix = new glm::mat4(1.0f);
	else
	{
		this->transformMatrix = new glm::mat4(*transformMatrix);
	}
}

CSceneGraphNodeTransform::~CSceneGraphNodeTransform()
{
	if (transformMatrix) delete transformMatrix;
}

const glm::mat4*	CSceneGraphNodeTransform::getTansformMatrix()const
{
	return transformMatrix;
}

void CSceneGraphNodeTransform::setTansformMatrix(const glm::mat4* transformMatrix)
{
	if (this->transformMatrix) delete this->transformMatrix;
	this->transformMatrix = new glm::mat4(*transformMatrix);
}

/////////////////////////////////////////////////////////////////////

CSceneGraphNodeDrawableBase::CSceneGraphNodeDrawableBase(const std::string name, CFrameBuffer* FBO, CGLShaderWarper* shader, CAdditionalParams* params) : CSceneGraphNode(CSceneGraph::GR_NODE_TYPE_DRAWABLEBASE, name)
{
	this->FBO = FBO;
	this->shader = shader;
	this->params = params;
	if (!this->params) this->params = new CAdditionalParams();
}

CSceneGraphNodeDrawableBase::~CSceneGraphNodeDrawableBase()
{

}

void CSceneGraphNodeDrawableBase::setFBO(CFrameBuffer* FBO)
{
	this->FBO = FBO;
}

const CFrameBuffer* CSceneGraphNodeDrawableBase::getFBO()const
{
	return FBO;
}

void CSceneGraphNodeDrawableBase::setParams(CAdditionalParams* params)
{
	this->params = params;
}

const CAdditionalParams* CSceneGraphNodeDrawableBase::getParams()const
{
	return params;
}

void CSceneGraphNodeDrawableBase::setShaderWarper(CGLShaderWarper* shader)
{
	this->shader = shader;
}

const CGLShaderWarper* CSceneGraphNodeDrawableBase::getShaderWarper()const
{
	return shader;
}

const bool CSceneGraphNodeDrawableBase::doWork()
{
	return true;
}

/////////////////////////////////////////////////////////////////////

CSceneGraphNodeDrawable::CSceneGraphNodeDrawable(const std::string name, CVertexBuffer *VBO) : CSceneGraphNode(CSceneGraph::GR_NODE_TYPE_DRAWABLE, name), CGameObj(TYPE_DRAWABLE)
{
	this->VBO = VBO;
	if (!this->VBO)this->VBO = new CVertexBuffer();
	onScreen = true;

	setHasUpdater(true);
}

CSceneGraphNodeDrawable::~CSceneGraphNodeDrawable()
{
	//if (VBO)
	//{
	//	VBO->cleanup();
	//	delete VBO;
	//	this->VBO = NULL;
	//}
}

const bool CSceneGraphNodeDrawable::doWork()
{
	return true;
}

const CVertexBuffer* CSceneGraphNodeDrawable::getVBO()const
{
	return VBO;
}

CVertexBuffer* CSceneGraphNodeDrawable::getVBOForModify()const
{
	return VBO;
}

void CSceneGraphNodeDrawable::setVBO(CVertexBuffer* VBO)
{
	//if (this->VBO)
	//{
	//	this->VBO->cleanup();
	//	delete this->VBO;
	//	this->VBO = NULL;
	//}
	this->VBO = VBO;
}

const bool CSceneGraphNodeDrawable::isOnScreen()const
{
	return onScreen;
}
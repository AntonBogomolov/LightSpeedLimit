#include "Scene.h"
#include "SceneGraphNode.h"
#include "SceneGraph.h"
#include "global.h"
#include "VideoManeger.h"
#include "GLMatrixSet.h"
#include "FrameBuffer.h"
#include "VertexBuffer.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "TextureAtlasManager.h"
#include "GLShaderObject.h"
#include "ShaderWarper.h"
#include "Camera.h"
#include "GameObj.h"
#include "SyncHelper.h"

CScene::CScene()
{
	rootNode = (CSceneGraphNode*) new CSceneGraphNodeGroup();

	nodesListHelperForDraw = new std::vector<CSceneGraphNode*>();
	nodesListHelperForDraw->reserve(CSceneGraphNode::MAX_CHILDS_PER_NODE);
	nodesListHelperForUpdate = new std::vector<CSceneGraphNode*>();
	nodesListHelperForUpdate->reserve(CSceneGraphNode::MAX_CHILDS_PER_NODE);

	drawableModifier = new CDrawableModifier();
	glMatrix = new CGLMatrixSet();

	needToInit = true;
}

CScene::~CScene()
{		
	delete nodesListHelperForDraw;
	delete nodesListHelperForUpdate;
	delete drawableModifier;
	delete glMatrix;	
}

const CSceneGraphNode* CScene::getRootNode()const
{
	return rootNode;
}

void CScene::draw()
{
	if (needToInit) return;	

	nodesListHelperForDraw->clear();
	nodesListHelperForDraw->insert(nodesListHelperForDraw->end(), rootNode->GetChilds()->rbegin(), rootNode->GetChilds()->rend());

	glm::mat4x4 modelMatrix = glm::mat4(1.0f);
	const CGLShaderWarper* modShader = NULL;
	const CAdditionalParams* modParams = NULL;

	CSceneGraphNodeDrawableBase* node = NULL;
	CSceneGraphNodeDrawable* drawableNode = NULL;
	CSceneGraphNodeTransform* nodeTransform = NULL;

	CSceneGraphNode* currNode;
	while (!nodesListHelperForDraw->empty())
	{
		currNode = nodesListHelperForDraw->back();
		nodesListHelperForDraw->pop_back();
		drawableModifier->freeModifiers(nodesListHelperForDraw->size());

		switch (currNode->getNodeType())
		{
		case CSceneGraph::GR_NODE_TYPE_GROUP:
			nodesListHelperForDraw->insert(nodesListHelperForDraw->end(), currNode->GetChilds()->rbegin(), currNode->GetChilds()->rend());
		break;
		case CSceneGraph::GR_NODE_TYPE_DRAWABLEBASE:
			node = (CSceneGraphNodeDrawableBase*)currNode;
			drawableModifier->addData(nodesListHelperForDraw->size(), glm::mat4(1.0f), node->getShaderWarper(), node->getFBO(), node->getParams());
			nodesListHelperForDraw->insert(nodesListHelperForDraw->end(), currNode->GetChilds()->rbegin(), currNode->GetChilds()->rend());
		break;
		case CSceneGraph::GR_NODE_TYPE_DRAWABLE:
			drawableNode = (CSceneGraphNodeDrawable*)currNode;
			if (!drawableNode->isOnScreen()) break;
						
			modShader	= drawableModifier->getLastShader();			
			modParams	= drawableModifier->getLastParams();
			
			drawNode(drawableNode, &modelMatrix, modShader, NULL, modParams);
		break;
		case CSceneGraph::GR_NODE_TYPE_TRANSFORM:
			nodeTransform = (CSceneGraphNodeTransform*)currNode;
			drawableModifier->addData(nodesListHelperForDraw->size(), *nodeTransform->getTansformMatrix());
			nodesListHelperForDraw->insert(nodesListHelperForDraw->end(), currNode->GetChilds()->rbegin(), currNode->GetChilds()->rend());
			break;
		default:
			break;
		}
	}
}

void CScene::drawNode(	const CSceneGraphNodeDrawable* drawableNode, const glm::mat4* modelMatrix, const CGLShaderWarper* shader,
						const CFrameBuffer* FBO, const CAdditionalParams* additionalParams)
{
	if (shader == NULL) return;
	glm::mat4 MMatrix = (*modelMatrix);
	glm::vec2 camCoords;

	int disableCode = 0;
	if (additionalParams)
		switch (additionalParams->paramID)
	{
		case CAdditionalParams::DR_PARAM_BLEND:
			disableCode = GL_BLEND;
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case CAdditionalParams::DR_PARAM_LOCKTOSCREEN:
			disableCode = -1;				
			camCoords = CCamera::getInstance()->getCamCoords();		
			MMatrix = glm::translate((*modelMatrix), glm::vec3(camCoords.x, camCoords.y, 0));
			break;
		default:
			break;
	}
	
	const CGLShaderWarper* currShaderWarper = shader;
	const CGLShaderObject* currShader = currShaderWarper->getShader();
	
	CSyncHelper::getInstance()->getLogicMutex()->lock();
	glMatrix->mMVPMatrix = glMatrix->mProjMatrix * glMatrix->mVMatrix * MMatrix;
	currShader->enable();
	currShader->linkModelViewProjectionMatrix(glMatrix->getMVPfloatPointer());		
	currShaderWarper->bindAllTextures();	
	currShaderWarper->updateShader();
	CSyncHelper::getInstance()->getLogicMutex()->unlock();

	const CVertexBuffer* VBO = drawableNode->getVBO();
	if (VBO) VBO->drawElements(0, currShader);

	if (additionalParams && disableCode > 0) glDisable(disableCode);

	CVertexBuffer::unbind();
	CTexture::unbind();
}

void CScene::updateScene(const float dt)
{
	if (needToInit)
	{		
		initScene();
		needToInit = false;		
	}

	const glm::vec2 camCoord = CCamera::getInstance()->getCamCoords();	
	glMatrix->mVMatrix = glm::lookAt(glm::vec3(camCoord.x, camCoord.y, 10.0f), glm::vec3(camCoord.x, camCoord.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	nodesListHelperForUpdate->clear();
	nodesListHelperForUpdate->insert(nodesListHelperForUpdate->end(), rootNode->GetChilds()->rbegin(), rootNode->GetChilds()->rend());
	
	CSceneGraphNode* currNode;
	while (!nodesListHelperForUpdate->empty())
	{	
		currNode = nodesListHelperForUpdate->back();
		nodesListHelperForUpdate->pop_back();
	
		if (currNode->getHasUpdater()) (dynamic_cast<CGameObj*>(currNode))->update(dt);

		switch (currNode->getNodeType())
		{	
			case CSceneGraph::GR_NODE_TYPE_GROUP:
				nodesListHelperForUpdate->insert(nodesListHelperForUpdate->end(), currNode->GetChilds()->rbegin(), currNode->GetChilds()->rend());
			break;
			case CSceneGraph::GR_NODE_TYPE_DRAWABLEBASE:		
				nodesListHelperForUpdate->insert(nodesListHelperForUpdate->end(), currNode->GetChilds()->rbegin(), currNode->GetChilds()->rend());
			break;
			case CSceneGraph::GR_NODE_TYPE_DRAWABLE:		
				//(dynamic_cast<CGameObj*>(currNode))->update(dt);				
			break;	
			case CSceneGraph::GR_NODE_TYPE_TRANSFORM:			
				nodesListHelperForUpdate->insert(nodesListHelperForUpdate->end(), currNode->GetChilds()->rbegin(), currNode->GetChilds()->rend());
			break;
			default:
			break;
		}
	}	
}

void CScene::clearScene()
{
	nodesListHelperForUpdate->clear();
	nodesListHelperForDraw->clear();
	drawableModifier->clearData();	

	CVideoManeger::GetInstance()->myGUI->destroyAllChildWidget();
}

void CScene::pauseScene()
{

}

void CScene::resumeScene()
{

}

void CScene::initScene()
{
	nodesListHelperForUpdate->clear();
	nodesListHelperForDraw->clear();
	drawableModifier->clearData();
	
	glMatrix->mVMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));		// View Matrix
	glMatrix->mProjMatrix = glm::ortho(0.0f, (float)CVideoManeger::scrWidth, (float)CVideoManeger::scrHeight, 0.0f, 0.1f, 100.0f);	// Proj Matrix

	glMatrix->mMMatrix = glm::mat4(1.0f);																							// Model Matrix
	glMatrix->mMVMatrix = glMatrix->mVMatrix * glMatrix->mMMatrix;
	glMatrix->mMVPMatrix = glMatrix->mProjMatrix * glMatrix->mVMatrix * glMatrix->mMMatrix;											// MVP Matrix

	loadResources();
	initUI();

	needToInit = false;
}

void CScene::clearGameObjects()
{

}
void CScene::initGameObjects()
{

}

void CScene::resizeScene()
{
	float zoom = CCamera::getInstance()->getZoom();

	glMatrix->mMMatrix = glm::mat4(1.0f);
	glMatrix->mVMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, zoom), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glMatrix->mProjMatrix = glm::ortho(0.0f, (float)CVideoManeger::scrWidth, (float)CVideoManeger::scrHeight, 0.0f, 0.1f, 100.0f);	// Proj Matrix
	glMatrix->mMVMatrix = glMatrix->mVMatrix * glMatrix->mMMatrix;
	glMatrix->mMVPMatrix = glMatrix->mProjMatrix * glMatrix->mVMatrix * glMatrix->mMMatrix;																// MVP Matrix
}

////////////////////////////////////////////////////////////////////////////////


CAdditionalParams::CAdditionalParams(int paramID)
{
	this->paramID = paramID;
}

CDrawableModifier::CDrawableModifier()
{
	modShader = new std::map<int, const CGLShaderWarper*>();
	modFBO = new std::map<int, const CFrameBuffer*>();
	modParams = new std::map<int, const CAdditionalParams*>();
}

// TO DO: fix this
CDrawableModifier::~CDrawableModifier()
{
	if (modShader) delete modShader;
	if (modFBO) delete modFBO;
	if (modParams) delete modParams;
}


const bool CDrawableModifier::isEmpty()
{
	if (modShader->empty() && modFBO->empty() && modParams->empty()) return true;
	return false;
}

void CDrawableModifier::clearData()
{
	modShader->clear();
	modFBO->clear();
	modParams->clear();
}

void CDrawableModifier::freeModifiers(const int nodeID)
{
	if (!modShader->empty())
	{
		auto it = modShader->begin();
		while (it != modShader->end())
		{
			if (it->first <= nodeID)
			{
				++it;
				continue;
			}
			modShader->erase(it++);
		}
	}
	if (!modFBO->empty())
	{
		auto it = modFBO->begin();
		while (it != modFBO->end())
		{
			if (it->first <= nodeID)
			{
				++it;
				continue;
			}
			it->second->unbind();
			modFBO->erase(it++);
			if (it != modFBO->end() && it->second)
			{
				it->second->bindOutput();
			}
		}
	}
	if (!modParams->empty())
	{
		auto it = modParams->begin();
		while (it != modParams->end())
		{
			if (it->first <= nodeID)
			{
				++it;
				continue;
			}
			modParams->erase(it++);
		}
	}
}

void CDrawableModifier::addData(const int nodeID, const glm::mat4 modMatrix, const CGLShaderWarper* modShader,
								const CFrameBuffer* modeFBO, const CAdditionalParams* additionalParams)
{
	if (modShader)
	{
		(*this->modShader)[nodeID] = modShader;		
	}
	if (modeFBO)
	{
		(*this->modFBO)[nodeID] = modeFBO;
		(*this->modFBO)[nodeID]->bindOutput();
	}
	if (additionalParams)		(*this->modParams)[nodeID]	= additionalParams;
}

const CGLShaderWarper* CDrawableModifier::getLastShader()const
{
	if (modShader->empty()) return NULL;

	return modShader->rbegin()->second;
}

const CGLShaderWarper* CDrawableModifier::popLastShader()
{
	if (modShader->empty()) return NULL;

	const CGLShaderWarper* retShader = modShader->rbegin()->second;
	modShader->erase(--modShader->end());
	return retShader;
}

const CFrameBuffer* CDrawableModifier::getLastFBO()const
{
	if (modFBO->empty()) return NULL;

	return modFBO->rbegin()->second;
}

const CFrameBuffer* CDrawableModifier::popLastFBO()
{
	if (modFBO->empty()) return NULL;

	const CFrameBuffer* retFBO = modFBO->rbegin()->second;
	modFBO->erase(--modFBO->end());
	return retFBO;
}

const CAdditionalParams* CDrawableModifier::getLastParams()const
{
	if (modParams->empty()) return NULL;

	return modParams->rbegin()->second;
}

const CAdditionalParams* CDrawableModifier::popLastParams()
{
	if (modParams->empty()) return NULL;

	const CAdditionalParams* retParams = modParams->rbegin()->second;
	modParams->erase(--modParams->end());
	return retParams;
}
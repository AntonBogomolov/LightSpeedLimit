#include "SceneGraph.h"
#include "SceneGraphNode.h"


CSceneGraph::CSceneGraph()
{
}


CSceneGraph::~CSceneGraph()
{
}

CSceneGraphNode* CSceneGraph::findNodeByName(CSceneGraphNode* rootNode, std::string nodeName)
{
	if (rootNode == NULL) return NULL;
	
	if (rootNode->getNodeName().compare(nodeName) == 0) return rootNode;

	CSceneGraphNode* result = NULL;
	for each (CSceneGraphNode* childNode in *rootNode->GetChilds())
	{
		result = findNodeByName(childNode, nodeName);
		if (result) return result;
	}

	return NULL;
}
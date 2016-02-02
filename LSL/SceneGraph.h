#pragma once
#include <string>

class CSceneGraphNode;

class CSceneGraph
{
public:
	static const int GR_NODE_TYPE_GROUP		= 0;
	static const int GR_NODE_TYPE_TRANSFORM = 1;
	static const int GR_NODE_TYPE_DRAWABLE	= 2;
	static const int GR_NODE_TYPE_DRAWABLEBASE = 3;
public:
	static CSceneGraphNode* findNodeByName(CSceneGraphNode* rootNode, std::string nodeName);

private:
	CSceneGraph();
	~CSceneGraph();
};

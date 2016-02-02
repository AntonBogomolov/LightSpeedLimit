#pragma once
#include "SceneGraphNode.h"
#include "VideoManeger.h"

class CDrawablePrimitive :
	public CSceneGraphNodeDrawable
{
private:
	int x;
	int y;
	int width;
	int height;
public:
	virtual void update(const float dt);
	virtual void updateManuallyVBO();

	CDrawablePrimitive(int x = 0, int y = 0, int width = CVideoManeger::scrWidth, int height = CVideoManeger::scrHeight);
	virtual ~CDrawablePrimitive();
};


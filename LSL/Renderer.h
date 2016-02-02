#pragma once
#include "IRender.h"
class CRenderer :public IRender
{
public:
	virtual void render(IScene* scene);
	virtual void initRender();
	virtual void cleanupRender();

	CRenderer();
	virtual ~CRenderer();
};


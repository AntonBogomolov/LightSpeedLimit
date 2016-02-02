#pragma once

class IScene;

class IRender
{
public:
	virtual void render(IScene* scene) = 0;
	virtual void initRender() = 0;
	virtual void cleanupRender() = 0;

	virtual ~IRender() { }
};


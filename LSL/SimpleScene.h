#pragma once
#include "Scene.h"

class CSimpleScene :
	public CScene
{
private:
	virtual void loadResources();
	virtual void clearResources();
	virtual void initUI();
	virtual void initGameObjects();
public:
	virtual void initScene();
	virtual void clearScene();
	virtual void updateScene(const float dt);
	virtual void inputEventHandler(const CInputEvent event);
	virtual void appEventHandler(const CGameEvent* event);
	virtual void uiEvents(MyGUI::WidgetPtr _sender);
	virtual void resizeScene();
public:
	CSimpleScene();
	virtual ~CSimpleScene();
};


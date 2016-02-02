#pragma once
#include "Scene.h"

class CSprite;

class CMainMenu : public CScene
{
private:
	CSprite*		bgrdSprite;
	CFrameBuffer*	FBO;
private:
	virtual void loadResources();
	virtual void clearResources();
	virtual void initUI();

	void initBackground();
	void updateShaders();
	void initShaders();
	virtual void initGameObjects();

public:
	virtual void initScene();
	virtual void clearScene();	
	virtual void updateScene(const float dt);
	virtual void resizeScene();
	virtual void inputEventHandler(const CInputEvent event);
	virtual void appEventHandler(const CGameEvent* event);

	virtual void pauseScene();
	virtual void resumeScene();

	virtual void uiEvents(MyGUI::WidgetPtr _sender);

	CMainMenu();
	virtual ~CMainMenu();
};


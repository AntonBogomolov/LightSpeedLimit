#pragma once
#include "Scene.h"

class CSprite;
class CPhysicsSolver;
class CStarSystem;

class CStarSystemScene : public CScene
{
private:
	CSprite*		bgrdSprite;
	CFrameBuffer*	FBO;
	CPhysicsSolver* phySolver;
	CStarSystem*	starSystem;
private:
	virtual void loadResources();
	virtual void clearResources();
	virtual void initUI();

	void initStarSystem();
	void initBackground();
	void updateShaders();
	void initShaders();
	virtual void initGameObjects();
public:
	virtual void initScene();
	virtual void clearScene();
	virtual void updateScene(const float dt);
	virtual void resizeScene();

	virtual void pauseScene();
	virtual void resumeScene();

	virtual void inputEventHandler(const CInputEvent event);
	virtual void appEventHandler(const CGameEvent* event);
	virtual void uiEvents(MyGUI::WidgetPtr _sender);
public:
	CStarSystemScene();
	virtual ~CStarSystemScene();
};


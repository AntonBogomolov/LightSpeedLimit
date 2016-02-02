#pragma once
#include "IInputHandler.h"
#include "IGameEventHandler.h"

#include "../../tools/includes/mygui/MyGUI.h"

class CGameEvent;

class IScene : public IInputHandler, public IGameEventHandler
{
protected:
	virtual void loadResources() = 0;
	virtual void clearResources() = 0;
	virtual void initUI() = 0;
public:
	virtual void draw() = 0;
	virtual void clearScene() = 0;
	virtual void initScene() = 0;
	virtual void updateScene(const float dt) = 0;
	virtual void uiEvents(MyGUI::WidgetPtr _sender) = 0;

	virtual ~IScene(){};
};


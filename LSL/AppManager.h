#pragma once
#include "AppBase.h"

class CMainMenu;

class CAppManager : public CAppBase
{
protected:		
	virtual void appEvents(const float dt);
	virtual void inputEventHandler(const CInputEvent event);
public:
	virtual void Init();

	CAppManager(void);
	virtual ~CAppManager(void);
};

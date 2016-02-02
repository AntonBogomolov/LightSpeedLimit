#pragma once
#include "IInputHandler.h"
#include <thread>
#include <chrono>

class CVideoManeger;
class CInputManager;
class CFrameTimer;
class CFrameBuffer;
class CGLMatrixSet;
class CScene;
class CRenderer;

class CAppBase : public IInputHandler
{
protected:
	bool quit;
	
	CVideoManeger*	VideoManeger;
	CInputManager*	InputManeger;
		
	CScene*		currScene;
	CRenderer*	openGLRenderer;

	std::thread* drawThread;
	std::thread* logicThread;
private:
	virtual void DrawLoop();
	virtual void LogicLoop();
	virtual void Draw();
	virtual void DrawUI();
	virtual void InitVideo();
	virtual void initGUI();
protected:
	virtual void appEvents(const float dt) = 0;
	virtual void changeScene(CScene* newScene);
public:	
	virtual void Init(); 
	virtual void Run();

	const bool isWorking() const;
public:
	CAppBase();
	virtual ~CAppBase();
};


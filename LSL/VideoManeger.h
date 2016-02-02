#pragma once

#include "global.h"

class CVideoManeger
{
private:
	static CVideoManeger* instance;
	CVideoManeger(void);
public:
	SDL_Event event;
	static int scrWidth;
	static int scrHeight;
	int bpp ;
	int flags;	

	SDL_GLContext		glRenderContext;
	SDL_GLContext		glMainContext;
	SDL_GLContext		glThreadContext;
	SDL_Window*			dispWindow;
	SDL_Renderer*		dispRenderer;
	SDL_RendererInfo	dispRendererInfo;
	
	MyGUI::OpenGLPlatform* uiPlatform;
	MyGUI::Gui* myGUI;

	void InitSDL();
	void InitDrawThread();

	static CVideoManeger* GetInstance();
	~CVideoManeger(void);
};
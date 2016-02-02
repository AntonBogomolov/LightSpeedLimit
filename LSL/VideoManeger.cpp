#include "VideoManeger.h"
#include "global.h"
#include "EngineDefines.h"
#include "Log.h"

CVideoManeger* CVideoManeger::instance=NULL;
int CVideoManeger::scrWidth = 1600;
int CVideoManeger::scrHeight = 800;

CVideoManeger::CVideoManeger(void)
{
	bpp = 0;
	flags = 0;
}

CVideoManeger::~CVideoManeger(void)
{
	myGUI->shutdown();
	delete myGUI;
	uiPlatform->shutdown();
	delete uiPlatform;
	
	SDL_GL_DeleteContext(glThreadContext);
	SDL_GL_DeleteContext(glRenderContext);
	SDL_DestroyWindow(dispWindow);
}

void CVideoManeger::InitSDL()
{
	// ----- SDL init ---------------
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) < 0)
	{
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| Video initialization failed: ") + SDL_GetError());
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}	
	atexit(SDL_Quit);

	// ----- OpenGL attribute setting via SDL ---------------
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
	
	if (SDL_CreateWindowAndRenderer(scrWidth, scrHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE, &dispWindow, &dispRenderer) == -1)
	{		
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| SDL_CreateWindowAndRenderer failed: ") + SDL_GetError());
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}	
	glThreadContext = SDL_GL_CreateContext(dispWindow);
	if (!glThreadContext)
	{		
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| SDL_GL_CreateContext failed: ") + SDL_GetError());
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}
	glRenderContext = SDL_GL_CreateContext(dispWindow);
	if (!glRenderContext)
	{
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| SDL_GL_CreateContext failed: ") + SDL_GetError());
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}
	glMainContext = SDL_GL_CreateContext(dispWindow);
	if (!glMainContext)
	{
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| SDL_GL_CreateContext failed: ") + SDL_GetError());
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}
	SDL_GL_MakeCurrent(dispWindow, glMainContext);

	SDL_GetRendererInfo(dispRenderer, &dispRendererInfo);
	if ((dispRendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||	(dispRendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0) 
	{		
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| Video initialization failed: ") + SDL_GetError());
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}
		
	SDL_GL_SetSwapInterval(0);
	SDL_SetWindowTitle(dispWindow, "LSL");

	// ---- GLEW init ---------------
	GLenum err = glewInit();

	if (GLEW_OK != err)
	{		
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| GLEW error: ") + ((char*)glewGetErrorString(err)));
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}

	if (!GLEW_VERSION_3_0)
	{
		//std::cout << "OpenGL3 is not supported";
		CLog::getInstance()->addError("CVideoManeger::InitSDL() \t| OpenGL3 is not supported");
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}
	// ---- TTF ---------------------

	TTF_Init();
	atexit(TTF_Quit);

	// ----SDL_image_----------------	

	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{		
		CLog::getInstance()->addError(std::string("CVideoManeger::InitSDL() \t| SDL_image could not initialize! SDL_image Error: ") + IMG_GetError());
		CLog::getInstance()->saveLogFile();
		exit(-1);
	}
	atexit(IMG_Quit);

	SDL_Surface* icon = IMG_Load("media\\icon.png");
	if (icon)
	{
		SDL_SetWindowIcon(CVideoManeger::GetInstance()->dispWindow, icon);
		SDL_FreeSurface(icon);
	}	
}

void CVideoManeger::InitDrawThread()
{
	
}

CVideoManeger* CVideoManeger::GetInstance()
{
	if(instance == NULL)
	{
		instance = new CVideoManeger();		
	}
	return instance;
}
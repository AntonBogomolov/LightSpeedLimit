#include "MainMenu.h"
#include "VideoManeger.h"
#include "InputManager.h"
#include "global.h"
#include "FrameTimer.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "TextureAtlasManager.h"
#include "GLShaderObject.h"
#include "ShaderWarper.h"
#include "ShaderManager.h"
#include "FrameBuffer.h"
#include "GLMatrixSet.h"
#include "GLUtils.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "EventsManager.h"
#include "GameEvent.h"
#include "SyncHelper.h"
#include "AppDefines.h"

#include "Scene.h"
#include "SceneGraph.h"
#include "SceneGraphNode.h"

#include "Camera.h"
#include "Sprite.h"
#include "SpriteCollection.h"
#include "AnimatedSprite.h"

CSpriteCollection* spriteCol;
const int sprCnt = 0;

CMainMenu::CMainMenu() : CScene()
{

}

CMainMenu::~CMainMenu()
{
	clearResources();
	clearScene();
}

void CMainMenu::initScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	__super::initScene();

	//--------------------------------			
	CCamera::getInstance()->setCamVelocity(0.0f, 0.0f);
	CCamera::getInstance()->setCamCoords(-CVideoManeger::scrWidth / 2, -CVideoManeger::scrHeight / 2);	
	//// INIT FBO ----------------------
	//CFrameBuffer::unbind();
	//if (FBO) delete FBO;
	//FBO = new CFrameBuffer();
	//FBO->setSize(CVideoManeger::scrWidth, CVideoManeger::scrHeight);
	//FBO->attachTexture(GL_RGBA, GL_LINEAR);
	//CFrameBuffer::unbind();
	////--------------------------------		

	//CEventsManager::addEvent(new CGameEvent(Defines::EV_INIT_UI));

	initBackground();
	initShaders();
	initGameObjects();	
	
	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CMainMenu::initGameObjects()
{
	CGLShaderWarper* twBackground = new CGLShaderWarper(CShaderManager::getShader(Defines::SH_STARS_BACKGROUND), STextureDescriptor::TEXD_ATLAS, Defines::ATLAS_BGRND_BIG);
		
	CSceneGraphNodeDrawableBase* dnBackground = new CSceneGraphNodeDrawableBase("quadBaseFBO", NULL, twBackground, new CAdditionalParams(CAdditionalParams::DR_PARAM_LOCKTOSCREEN));
	dnBackground->AddChild(bgrdSprite);
	
	rootNode->AddChild(dnBackground);
}

void  CMainMenu::initShaders()
{
	const glm::vec2 camCoord = CCamera::getInstance()->getCamCoords();
	float camX = ((camCoord.x + 1000.0f) / 2000.0f) / 15.0f;
	float camY = ((camCoord.y + 1000.0f) / 2000.0f) / 15.0f;

	CGLShaderObject* starsBackground = CShaderManager::getShader(Defines::SH_STARS_BACKGROUND);
	starsBackground->enable();

	const CTexCoord* clTex = CTextureAtlasManager::getAtlas(Defines::ATLAS_BGRND_BIG)->getTexCoord(Defines::TEX_BGRND_1);
	const CTexCoord* medTex = CTextureAtlasManager::getAtlas(Defines::ATLAS_BGRND_BIG)->getTexCoord(Defines::TEX_BGRND_2);
	const CTexCoord* farTex = CTextureAtlasManager::getAtlas(Defines::ATLAS_BGRND_BIG)->getTexCoord(Defines::TEX_BGRND_3);

	float farTexX = farTex->tx;
	float farTexY = farTex->ty;
	float farTexW = farTex->twidth;
	float farTexH = farTex->theight;

	float medTexX = medTex->tx;
	float medTexY = medTex->ty;
	float medTexW = medTex->twidth;
	float medTexH = medTex->theight;

	float clTexX = clTex->tx;
	float clTexY = clTex->ty;
	float clTexW = clTex->twidth;
	float clTexH = clTex->theight;

	starsBackground->setUniformParameter2f("farTexXY", farTexX, farTexY);
	starsBackground->setUniformParameter2f("farTexWH", farTexW, farTexH);

	starsBackground->setUniformParameter2f("medTexXY", medTexX, medTexY);
	starsBackground->setUniformParameter2f("medTexWH", medTexW, medTexH);

	starsBackground->setUniformParameter2f("clTexXY", clTexX, clTexY);
	starsBackground->setUniformParameter2f("clTexWH", clTexW, clTexH);

	starsBackground->setUniformParameter2f("camPos", camX, camY);

	starsBackground->setUniformParameter1f("farScale", 2.0f);
	starsBackground->setUniformParameter1f("MedScale", 2.5f);
	starsBackground->setUniformParameter1f("ClScale", 3.0f);
	starsBackground->setUniformParameter1f("whRaito", (float)CVideoManeger::scrWidth / CVideoManeger::scrHeight);
	starsBackground->setUniformParameter1i("sTexture", 0);

	starsBackground->disable();
}

void CMainMenu::initBackground()
{
	bgrdSprite = new CSprite(Defines::ATLAS_BGRND_BIG, Defines::TEX_BGRND_1, CVideoManeger::scrWidth, CVideoManeger::scrHeight);	
	bgrdSprite->setCenterAbsoluteCoords(CVideoManeger::scrWidth / 2, CVideoManeger::scrHeight / 2);	
	bgrdSprite->isNeedToUpdateVBO(false);
}

void CMainMenu::clearScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	__super::clearScene();

	if (FBO)
	{
		FBO->cleanup();
		delete FBO;
	}
	if (bgrdSprite) delete bgrdSprite;
	
	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CMainMenu::loadResources()
{
	CShaderManager::loadShader(Defines::SH_TEXTURER, "media\\shaders\\texturerVert.glsl", "media\\shaders\\texturerFrag.glsl");
	CShaderManager::loadShader(Defines::SH_STARS_BACKGROUND, "media\\shaders\\starBackgroundVert.glsl", "media\\shaders\\starBackgroundFrag.glsl");

	CTextureAtlas* texAtlas = CTextureAtlasManager::addAtlas(Defines::ATLAS_BGRND_BIG);
	texAtlas->addTexture("media\\textures\\bg08.png", Defines::TEX_BGRND_3);
	texAtlas->addTexture("media\\textures\\bg03.png", Defines::TEX_BGRND_2);
	texAtlas->addTexture("media\\textures\\bg00.png", Defines::TEX_BGRND_1);
	texAtlas->addTexture("media\\textures\\bg07.png", Defines::TEX_BGRND_4);
	texAtlas->generateAtlas();
}

void CMainMenu::clearResources()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	CShaderManager::lazyCleanup();
	CTextureAtlasManager::lazyCleanup();

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CMainMenu::pauseScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	CFrameBuffer::unbind();
	FBO->deattachTextures();

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CMainMenu::resumeScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	//// REINIT FBO ----------------------
	//CFrameBuffer::unbind();
	//FBO->deattachTextures();
	//FBO->setSize(CVideoManeger::scrWidth, CVideoManeger::scrHeight);
	//FBO->attachTexture(GL_RGBA, GL_LINEAR);
	//CFrameBuffer::unbind();
	////--------------------------------		
	initShaders();

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CMainMenu::initUI()
{	
	MyGUI::WindowPtr uiWindow = CVideoManeger::GetInstance()->myGUI->createWidget<MyGUI::Window>("WindowCS", MyGUI::IntCoord(10, 10, 300, 400), MyGUI::Align::Center, "Main");
	
	//MyGUI::ITexture* texture = CVideoManeger::GetInstance()->uiPlatform->getRenderManagerPtr()->createTexture("logoTexture");

	MyGUI::ImageBox* logo = uiWindow->createWidget<MyGUI::ImageBox>("ImageBox", 10, 10, 200, 100, MyGUI::Align::Top, "Main");
	logo->setImageTexture("images//logo.png");

	MyGUI::ButtonPtr buttonNewGame = uiWindow->createWidget<MyGUI::Button>("Button", 10, 110, 200, 40, MyGUI::Align::Top, "Main");
	buttonNewGame->setCaption("New game");
	buttonNewGame->eventMouseButtonClick += MyGUI::newDelegate(this, &CMainMenu::uiEvents);
	buttonNewGame->setUserString("id","bNewGame");

	MyGUI::ButtonPtr buttonExit = uiWindow->createWidget<MyGUI::Button>("Button", 10, 160, 200, 40, MyGUI::Align::Top, "Main");
	buttonExit->setCaption("Exit");
	buttonExit->eventMouseButtonClick += MyGUI::newDelegate(this, &CMainMenu::uiEvents);
	buttonExit->setUserString("id", "bExit");

	uiWindow->setSize(MyGUI::IntSize(310,410)); // Align
	uiWindow->setSnap(true);
	uiWindow->setCaption("LSL");	

	CVideoManeger::GetInstance()->uiPlatform->getRenderManagerPtr()->setViewSize(CVideoManeger::GetInstance()->scrWidth, CVideoManeger::GetInstance()->scrHeight);
}

void CMainMenu::appEventHandler(const CGameEvent* event)
{
	switch (event->getEventId())
	{
		case Defines::EV_CAM_MOVE:
			updateShaders();
		break;		
		case Defines::EV_INIT_UI:
			initUI();
			break;
		default:		
		break;
	}
}

void CMainMenu::inputEventHandler(const CInputEvent event)
{

}

void CMainMenu::uiEvents(MyGUI::WidgetPtr _sender)
{	
	std::string senderIDString = _sender->getUserString("id");

	if (senderIDString.compare("bExit") == 0)		CEventsManager::addEvent(new CGameEvent(Defines::EV_APP_EXIT));
	if (senderIDString.compare("bNewGame") == 0)	CEventsManager::addEvent(new CGameEvent(Defines::EV_UI_NEWGAME));
}

void CMainMenu::updateShaders()
{
	const glm::vec2 camCoord = CCamera::getInstance()->getCamCoords();
	float farScale = 2.0f;
	float camX = ((camCoord.x + 1000.0f) / 2000.0f) / 15.0f;
	float camY = ((camCoord.y + 1000.0f) / 2000.0f) / 15.0f;
	CGLShaderObject* starsBackground = CShaderManager::getShader(Defines::SH_STARS_BACKGROUND);
	starsBackground->enable();
	starsBackground->setUniformParameter2f("camPos", camX, camY);
	starsBackground->disable();
}

void CMainMenu::updateScene(const float dt)
{
	__super::updateScene(dt);
}

void CMainMenu::resizeScene()
{
	CSyncHelper::getInstance()->getLogicMutex()->lock();

	__super::resizeScene();

	//// REINIT FBO ----------------------
	//CFrameBuffer::unbind();
	//FBO->deattachTextures();
	//FBO->setSize(CVideoManeger::scrWidth, CVideoManeger::scrHeight);
	//FBO->attachTexture(GL_RGBA, GL_LINEAR);
	//CFrameBuffer::unbind();
	////--------------------------------		
	bgrdSprite->setCenterAbsoluteCoords(CVideoManeger::scrWidth / 2, CVideoManeger::scrHeight / 2);
	bgrdSprite->setWidth(CVideoManeger::scrWidth);
	bgrdSprite->setHeight(CVideoManeger::scrHeight);	
	bgrdSprite->updateManuallyVBO();
	//CGLUtils::getFullscreenRevDrawableNode()->updateManuallyVBO();

	initShaders();
	updateShaders();

	CSyncHelper::getInstance()->getLogicMutex()->unlock();
}
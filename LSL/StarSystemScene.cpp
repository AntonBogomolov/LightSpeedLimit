#include "StarSystemScene.h"
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
#include "DrawablePrimitive.h"

#include "Scene.h"
#include "SceneGraph.h"
#include "SceneGraphNode.h"

#include "Camera.h"
#include "Sprite.h"
#include "SpriteCollection.h"
#include "AnimatedSprite.h"
#include "PhysicsSolver.h"
#include "Orbit.h"
#include "StarSystem.h"
#include "Planet.h"
#include "PlanetTextureGenerator.h"
#include "Star.h"

CStarSystemScene::CStarSystemScene() : CScene()
{

}

CStarSystemScene::~CStarSystemScene()
{
	clearResources();
	clearScene();
}

void CStarSystemScene::initScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	__super::initScene();
	//--------------------------------		
	CCamera::getInstance()->setCamCoords(-CVideoManeger::scrWidth / 2, -CVideoManeger::scrHeight / 2);
	CCamera::getInstance()->setCamVelocity(0.0f, 0.0f);
	//// INIT FBO ----------------------
	//CFrameBuffer::unbind();
	//FBO = new CFrameBuffer();	
	//FBO->setSize(CVideoManeger::scrWidth, CVideoManeger::scrHeight);
	//FBO->attachTexture(GL_RGBA, GL_LINEAR);
	//CFrameBuffer::unbind();
	////--------------------------------		
	initBackground();
	initStarSystem();
	initShaders();	
	//--------------------------------
	initGameObjects();
	//--------------------------------	

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CStarSystemScene::initGameObjects()
{	
	CGLShaderWarper* twBackground = new CGLShaderWarper(CShaderManager::getShader(Defines::SH_STARS_BACKGROUND), STextureDescriptor::TEXD_ATLAS, Defines::ATLAS_BGRND_BIG);
//	CGLShaderWarper* twFBOTexturer = new CGLShaderWarper(CShaderManager::getShader(Defines::SH_TEXTURER),STextureDescriptor::TEXD_TEXTURE, FBO->getTexId());
	CGLShaderWarper* twTexturer = new CGLShaderWarper(CShaderManager::getShader(Defines::SH_TEXTURER), STextureDescriptor::TEXD_ATLAS, Defines::ATLAS_STARSYSTEM);
					
	CSceneGraphNodeDrawableBase* dnBackground = new CSceneGraphNodeDrawableBase("quadBaseFBO", NULL, twBackground, new CAdditionalParams(CAdditionalParams::DR_PARAM_LOCKTOSCREEN));
	dnBackground->AddChild(bgrdSprite);	
	dnBackground->AddChild(starSystem->getDrawNode());

//	CSceneGraphNodeDrawableBase* dnFullscreenQuad = new CSceneGraphNodeDrawableBase("quadBase", NULL, twFBOTexturer, new CAdditionalParams(CAdditionalParams::DR_PARAM_LOCKTOSCREEN));
//	dnFullscreenQuad->AddChild(CGLUtils::getFullscreenRevDrawableNode());
		
	CSceneGraphNodeDrawableBase* dnTexture = new CSceneGraphNodeDrawableBase("", NULL, twTexturer, new CAdditionalParams(CAdditionalParams::DR_PARAM_LOCKTOSCREEN));
	dnTexture->AddChild(new CDrawablePrimitive(0, 0,768,768));
	
	rootNode->AddChild(dnBackground);	
//	rootNode->AddChild(dnTexture);
//	rootNode->AddChild(dnFullscreenQuad);	
}

void CStarSystemScene::initStarSystem()
{
	phySolver  = new CPhysicsSolver();	
	starSystem = new CStarSystem();

	CPlanet* tmpPlanet;
	CStar* star = new CStar("Sun", CStar::STAR_CLASS_G);
	star->setSprite(Defines::ATLAS_STARSYSTEM, Defines::TEX_STAR, 1, 1);
	star->getOrbit()->setIsNeedToCalcOrbit(false);
	
	CPlanet* planet = new CPlanet("Merc", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 50.76, 0, CPlanet::EARTH_MASS * 0.06, CPlanet::EARTH_RAD * 1.382);
	star->addSatellite(planet);

//	planet = new CPlanet("Merc2", CPlanet::PLANET_TYPE_EARTHLIKE, 28, -50.76, 0, CPlanet::EARTH_MASS * 60000.06, CPlanet::EARTH_RAD * 0.382);
//	star->addSatellite(planet);
//	planet->setSprite(Defines::ATLAS_STARSYSTEM, Defines::TEX_PLANET, 1, 1);
	
	planet = new CPlanet("Venus", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 107.28, 0, CPlanet::EARTH_MASS * 0.82, CPlanet::EARTH_RAD * 1.949);
	star->addSatellite(planet);
	
	planet = new CPlanet("Earth", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 149.0, 0);
	star->addSatellite(planet);
	tmpPlanet = planet;

	planet = new CPlanet("Moon", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 149.384, 0, CPlanet::EARTH_MASS * 0.0123, CPlanet::EARTH_RAD * 0.273);		
	tmpPlanet->addSatellite(planet);
	planet->getOrbit()->setIsNeedToCalcOrbit(false);
	
	planet = new CPlanet("Mars", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 226.5, 0, CPlanet::EARTH_MASS * 0.11, CPlanet::EARTH_RAD * 1.53);
	star->addSatellite(planet);
	
	planet = new CPlanet("Jupiter", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 774.000, 0, CPlanet::EARTH_MASS * 308.0, CPlanet::EARTH_RAD * 11.2);
	star->addSatellite(planet);

	planet = new CPlanet("Saturn", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 1421.600, 0, CPlanet::EARTH_MASS * 95.0, CPlanet::EARTH_RAD * 9.41);
	star->addSatellite(planet);
	
	planet = new CPlanet("Uranus", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 2863.780, 0, CPlanet::EARTH_MASS * 14.6, CPlanet::EARTH_RAD * 3.98);
	star->addSatellite(planet);
	
	planet = new CPlanet("Neptun", CPlanet::PLANET_TYPE_EARTHLIKE, 28, 4478.940, 0, CPlanet::EARTH_MASS * 17.2, CPlanet::EARTH_RAD * 3.81);
	star->addSatellite(planet);
	
//	CStar* star2 = new CStar("Sun2",4,0,0,0,-100.0,0);
//  star2->setSprite(Defines::ATLAS_STARSYSTEM, Defines::TEX_STAR, 1, 1);
//	star->addSatellite(star2);
		
	starSystem->addStar(star);	
	phySolver->addPhysicsObjects(starSystem->getAllPhysicsObjects());

	CPlanetTextureGenerator* planetTexGen = new CPlanetTextureGenerator();	
	CTextureAtlas* texAtlasStarSystem = CTextureAtlasManager::getAtlas(Defines::ATLAS_STARSYSTEM);
	int planetNumber = 1;
	for each (CPlanet* currPlanet in *starSystem->getPlanets())
	{
		currPlanet->setSprite(Defines::ATLAS_STARSYSTEM, Defines::TEX_PLANET + planetNumber, 1, 1);
		CTexture* planetTexture = planetTexGen->generatePlanetTexture(512, 512, currPlanet->getPlanetGeneratorParams());
		texAtlasStarSystem->addSpriteSet(planetTexture, currPlanet->getSprite()->getSpriteTextureID(), 1, 1);	
		planetNumber++;
	}
	texAtlasStarSystem->generateAtlas();
	delete planetTexGen;
}

void  CStarSystemScene::initShaders()
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

	// PLANETS

	CGLShaderObject* orbitShader = CShaderManager::getShader(Defines::SH_ORBITS);
	orbitShader->enable();
	orbitShader->setUniformParameter3f("orbitColor", 0.4f, 0.4f, 0.4f);
	orbitShader->disable();

	// STARS
	CGLShaderObject* starTexturer = CShaderManager::getShader(Defines::SH_STARS_TEXTURES);
	starTexturer->enable();
	starTexturer->setUniformParameter3f("starColor", 0.9f, 0.5f, 0.1f);	
	starTexturer->setUniformParameter1i("sTexture", 0);

	const CTexCoord* starTex = CTextureAtlasManager::getAtlas(Defines::ATLAS_STARSYSTEM)->getTexCoord(Defines::TEX_STAR);
	starTexturer->setUniformParameter2f("texSize", starTex->twidth, starTex->theight);
	starTexturer->setUniformParameter2f("texLeftCorner", starTex->tx, starTex->ty);
	starTexturer->disable();
}

void CStarSystemScene::initBackground()
{	
	bgrdSprite = new CSprite(Defines::ATLAS_BGRND_BIG, Defines::TEX_BGRND_1, CVideoManeger::scrWidth, CVideoManeger::scrHeight);
	bgrdSprite->setCenterAbsoluteCoords(CVideoManeger::scrWidth / 2, CVideoManeger::scrHeight / 2);
	bgrdSprite->setWidth(CVideoManeger::scrWidth);
	bgrdSprite->setHeight(CVideoManeger::scrHeight);
	bgrdSprite->isNeedToUpdateVBO(false);	
}

void CStarSystemScene::clearScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	__super::clearScene();

	if (FBO)
	{
		FBO->cleanup();
		delete FBO;
	}
	if (bgrdSprite) delete bgrdSprite;

	if (phySolver)
	{
		phySolver->stopSolverThread();
		delete phySolver;
	}
	if (starSystem) delete starSystem;	

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CStarSystemScene::loadResources()
{
	CShaderManager::loadShader(Defines::SH_TEXTURER,			"media\\shaders\\texturerVert.glsl",		"media\\shaders\\texturerFrag.glsl");
	CShaderManager::loadShader(Defines::SH_STARS_BACKGROUND,	"media\\shaders\\starBackgroundVert.glsl",	"media\\shaders\\starBackgroundFrag.glsl");
	CShaderManager::loadShader(Defines::SH_PLANET_TEXTURER,		"media\\shaders\\simplePlanetVert.glsl",	"media\\shaders\\simplePlanetFrag.glsl");
	CShaderManager::loadShader(Defines::SH_PLANET_TEXTURER_C,	"media\\shaders\\simplePlanetVertC.glsl",	"media\\shaders\\simplePlanetFragC.glsl");
	CShaderManager::loadShader(Defines::SH_PLANET_TEXTURER_L,	"media\\shaders\\simplePlanetVertL.glsl",	"media\\shaders\\simplePlanetFragL.glsl");
	CShaderManager::loadShader(Defines::SH_PLANET_TEXTURER_CL,	"media\\shaders\\simplePlanetVertCL.glsl",	"media\\shaders\\simplePlanetFragCL.glsl");

	CShaderManager::loadShader(Defines::SH_STARS_TEXTURES,		"media\\shaders\\simpleStarVert.glsl",		"media\\shaders\\simpleStarFrag.glsl");
	CShaderManager::loadShader(Defines::SH_ORBITS,				"media\\shaders\\linesVert.glsl",			"media\\shaders\\linesFrag.glsl");

	CTextureAtlas* texAtlas = CTextureAtlasManager::addAtlas(Defines::ATLAS_BGRND_BIG);
	texAtlas->addTexture("media\\textures\\bg08.png", Defines::TEX_BGRND_3);
	texAtlas->addTexture("media\\textures\\bg03.png", Defines::TEX_BGRND_2);
	texAtlas->addTexture("media\\textures\\bg00.png", Defines::TEX_BGRND_1);
	texAtlas->addTexture("media\\textures\\bg07.png", Defines::TEX_BGRND_4);
	texAtlas->generateAtlas();
	
	CTextureAtlas* texAtlasStarSystem = CTextureAtlasManager::addAtlas(Defines::ATLAS_STARSYSTEM);
	texAtlasStarSystem->addSpriteSet("media\\textures\\star1.png", Defines::TEX_STAR, 1,1);
	texAtlasStarSystem->addSpriteSet("media\\textures\\earth.png", Defines::TEX_PLANET, 1, 1);		
	//texAtlasStarSystem->generateAtlas();
}

void CStarSystemScene::clearResources()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	CShaderManager::lazyCleanup();

	CTextureAtlasManager::delAtlas(Defines::ATLAS_STARSYSTEM);
	CTextureAtlasManager::lazyCleanup();

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CStarSystemScene::pauseScene()
{
	CSyncHelper::getInstance()->getDrawMutex()->lock();

	CFrameBuffer::unbind();
	FBO->deattachTextures();

	CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CStarSystemScene::resumeScene()
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

void CStarSystemScene::initUI()
{
	MyGUI::WindowPtr uiWindow = CVideoManeger::GetInstance()->myGUI->createWidget<MyGUI::Window>("Window", MyGUI::IntCoord(10, 10, 300, 100), MyGUI::Align::Top | MyGUI::Align::Left, "Main");

	MyGUI::ButtonPtr buttonMenu = uiWindow->createWidget<MyGUI::Button>("Button", 10, 20, 100, 60, MyGUI::Align::Left, "Main");
	buttonMenu->setCaption("Menu");
	buttonMenu->eventMouseButtonClick += MyGUI::newDelegate(this, &CStarSystemScene::uiEvents);
	buttonMenu->setUserString("id", "bMenu");

	MyGUI::ButtonPtr buttonExit = uiWindow->createWidget<MyGUI::Button>("Button", 110, 20, 100, 60, MyGUI::Align::Left, "Main");
	buttonExit->setCaption("Exit");
	buttonExit->eventMouseButtonClick += MyGUI::newDelegate(this, &CStarSystemScene::uiEvents);
	buttonExit->setUserString("id", "bExit");

	uiWindow->setSize(MyGUI::IntSize(310, 100)); // Align
	uiWindow->setSnap(true);
	uiWindow->setCaption("LSL");
}

void CStarSystemScene::uiEvents(MyGUI::WidgetPtr _sender)
{
	std::string senderIDString = _sender->getUserString("id");

	if (senderIDString.compare("bExit") == 0)		CEventsManager::addEvent(new CGameEvent(Defines::EV_APP_EXIT));
	if (senderIDString.compare("bMenu") == 0)		CEventsManager::addEvent(new CGameEvent(Defines::EV_UI_MENU));
}

void CStarSystemScene::appEventHandler(const CGameEvent* event)
{
	switch (event->getEventId())
	{
	case Defines::EV_CAM_MOVE:
		updateShaders();
		break;
	case Defines::EV_CAM_ZOOM:
		//resizeScene();
		break;
	case Defines::EV_UPDATE_PHY_OBJ:
		for each (CSpaceObject* currObj in *starSystem->getAllSpaceObjectsForModify())
		{
			currObj->updateVBO();
		}
		break;
	default:
		break;
	}
}

void CStarSystemScene::inputEventHandler(const CInputEvent event)
{

}

void CStarSystemScene::updateShaders()
{
	//CSyncHelper::getInstance()->getLogicMutex()->lock();

	const glm::vec2 camCoord = CCamera::getInstance()->getCamCoords();
	float farScale = 2.0f;
	float camX = ((camCoord.x + 1000.0f) / 2000.0f) / 15.0f;
	float camY = ((camCoord.y + 1000.0f) / 2000.0f) / 15.0f;
	CGLShaderObject* starsBackground = CShaderManager::getShader(Defines::SH_STARS_BACKGROUND);
	starsBackground->enable();
	starsBackground->setUniformParameter2f("camPos", camX, camY);
	starsBackground->disable();

	//CSyncHelper::getInstance()->getLogicMutex()->unlock();
}

void CStarSystemScene::updateScene(const float dt)
{
	__super::updateScene(dt);
	if (!phySolver->getIsSolverThreadRuning()) phySolver->startSolverThread();
	//phySolver->solve(dt);
}

void CStarSystemScene::resizeScene()
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

	initShaders();
	updateShaders();

	CSyncHelper::getInstance()->getLogicMutex()->unlock();
}
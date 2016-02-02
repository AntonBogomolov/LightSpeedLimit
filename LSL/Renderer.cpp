#include "Renderer.h"
#include "Scene.h"
#include "global.h"

CRenderer::CRenderer()
{
}


CRenderer::~CRenderer()
{
}

void CRenderer::render(IScene* scene)
{
	if (!scene) return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->draw();
}

void CRenderer::initRender()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	//glDisable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_ALWAYS);
}

void CRenderer::cleanupRender()
{

}
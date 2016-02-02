#include "ShaderManager.h"
#include "GLShaderObject.h"

int			CShaderManager::shaderCount = 100;
CShaderManager*		CShaderManager::instance	= NULL;
std::map<int, CGLShaderObject*> CShaderManager::shaders;

CShaderManager::CShaderManager(int shCnt)
{
	shaderCount = shCnt;
	instance = NULL;
}

CShaderManager::~CShaderManager()
{
	cleanup();
}

CShaderManager* CShaderManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new CShaderManager();
	}
	return instance;
}

CGLShaderObject* CShaderManager::getShader(int shaderID)
{
	return shaders[shaderID];
}

void CShaderManager::loadShader(int shaderID, std::string shVertName, std::string shFragName)
{
		if (shaders[shaderID] == NULL)
		{
			shaders[shaderID] = new CGLShaderObject();
			shaders[shaderID]->init(shVertName, shFragName);
		}
		else
		{
			shaders[shaderID]->setIsUsing(true);
		}
}

void CShaderManager::cleanup()
{
	for each (auto currPair in shaders)
	{
		CGLShaderObject* currSh = currPair.second;
		if (currSh != NULL)
		{
			currSh->cleanup();
			delete currSh;
			currSh = NULL;
		}
	}
	shaders.clear();
}

void CShaderManager::lazyCleanup()
{
	for each (auto currPair in shaders)
	{
		CGLShaderObject* currSh = currPair.second;
		currSh->setIsUsing(false);
	}
}

void CShaderManager::cleanupUnused()
{
	for each (auto currPair in shaders)
	{
		CGLShaderObject* currSh = currPair.second;
		if (currSh)
		{
			if (currSh->getIsUsing()) continue;
			delete currSh;
			currSh = NULL;
		}
	}
}
#pragma once
#include <string>
#include <map>

class CGLShaderObject;

class CShaderManager
{
private:
	static int shaderCount;
	static CShaderManager* instance;

	CShaderManager(int shCnt = shaderCount);

public:
	//static CGLShaderObject** shaders;
	static std::map<int, CGLShaderObject*> shaders;

public:	
	static CShaderManager* getInstance();
	static CGLShaderObject* getShader(int shaderID);
	static void loadShader(int shaderID, std::string shVertName, std::string shFragName);
	
	static void cleanup();
	static void lazyCleanup();
	static void cleanupUnused();

	~CShaderManager();
};


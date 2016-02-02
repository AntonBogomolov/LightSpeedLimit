#pragma once
#include <vector>
#include "../../tools/includes/glm/glm.hpp"

class CGLShaderObject;

struct STextureDescriptor
{
	static const int TEXD_ATLAS = 0;
	static const int TEXD_TEXTURE = 1;

	int type;
	int texID;

	STextureDescriptor(int type, int texID);
};

class IShaderUpdater
{
private:
	mutable bool isNeedToUpdateShader;
	void* updaterParam;
public:
	const bool getIsNeedToUpdateShader() const;
	const void* getShaderUpdaterParam() const;
	void setShaderUpdaterParam(void* updaterParam);
	void setIsNeedToUpdateShader(bool needUpdate) const;

	virtual void updateShader() = 0;

	IShaderUpdater(); 
	virtual ~IShaderUpdater();
};

class CGLShaderWarper
{
private:
	CGLShaderObject* shader;
	IShaderUpdater* shaderUpdaterObj;
	std::vector<STextureDescriptor>* textures;
	glm::mat4* mMVPMatrix;
public:
	const CGLShaderObject* getShader()const;
	const std::vector<STextureDescriptor>* getTexturesDescriptor()const;

	void setShader(CGLShaderObject* newShader);
	void setShaderUpdater(IShaderUpdater* shaderUpdaterObj);
	void updateShader() const;
	void linkMVPMatrix(glm::mat4* mMVPMatrix);
	void clearTexturesDescriptor();
	void addTextureDescriptor(STextureDescriptor texDescriptor);
	void addTextureDescriptor(const int type, const int texID);
	const int getTexturesCnt()const;

	void bindAllTextures()const;

	CGLShaderWarper(CGLShaderObject* shader);
	CGLShaderWarper(CGLShaderObject* shader, int textureType, int textureID);
	~CGLShaderWarper();
};
#include "ShaderWarper.h"

#include "global.h"
#include "Texture.h"
#include "TextureAtlas.h"
#include "TextureAtlasManager.h"
#include "GLShaderObject.h"

STextureDescriptor::STextureDescriptor(int type, int texID)
{
	this->type = type;
	this->texID = texID;
}

//////////////////////////////////////////////////////////////////////////

IShaderUpdater::IShaderUpdater()
{
	isNeedToUpdateShader = true;
}

IShaderUpdater::~IShaderUpdater()
{

}

const bool IShaderUpdater::getIsNeedToUpdateShader() const
{
	return isNeedToUpdateShader;
}

void IShaderUpdater::setIsNeedToUpdateShader(bool needUpdate) const
{
	isNeedToUpdateShader = needUpdate;
}

const void* IShaderUpdater::getShaderUpdaterParam() const
{
	return updaterParam;
}

void IShaderUpdater::setShaderUpdaterParam(void* updaterParam)
{
	this->updaterParam = updaterParam;
}

//////////////////////////////////////////////////////////////////////////

CGLShaderWarper::CGLShaderWarper(CGLShaderObject* shader)
{
	if (shader) this->shader = shader;
	else this->shader = new CGLShaderObject();

	textures = new std::vector<STextureDescriptor>();
}

CGLShaderWarper::CGLShaderWarper(CGLShaderObject* shader, int textureType, int textureID)
{
	if (shader) this->shader = shader;
	else this->shader = new CGLShaderObject();

	textures = new std::vector<STextureDescriptor>();

	addTextureDescriptor(textureType, textureID);
}

CGLShaderWarper::~CGLShaderWarper()
{
	if (textures)
	{
		textures->clear();
		delete textures;
	}
}

void CGLShaderWarper::linkMVPMatrix(glm::mat4* mMVPMatrix)
{
	if (this->mMVPMatrix != mMVPMatrix)
	{
		this->mMVPMatrix = mMVPMatrix;
	}
	if (shader) this->shader->linkModelViewProjectionMatrix(&(*mMVPMatrix)[0][0]);
}

const CGLShaderObject* CGLShaderWarper::getShader()const
{
	return shader;
}

const std::vector<STextureDescriptor>* CGLShaderWarper::getTexturesDescriptor()const
{
	return textures;
}

void CGLShaderWarper::setShader(CGLShaderObject* newShader)
{
	if (newShader) this->shader = newShader;
}

void CGLShaderWarper::setShaderUpdater(IShaderUpdater* shaderUpdaterObj)
{
	this->shaderUpdaterObj = shaderUpdaterObj;
}

void CGLShaderWarper::updateShader() const
{
	if (shaderUpdaterObj)
	{
		if (!shaderUpdaterObj->getIsNeedToUpdateShader()) return;

		shaderUpdaterObj->updateShader();
	}
}

void CGLShaderWarper::clearTexturesDescriptor()
{
	if (textures)
	{
		textures->clear();
	}
}

void CGLShaderWarper::addTextureDescriptor(STextureDescriptor texDescriptor)
{
	if (textures)
	{
		textures->push_back(texDescriptor);
	}
}

void CGLShaderWarper::addTextureDescriptor(int type, int texID)
{
	if (textures)
	{
		textures->push_back(STextureDescriptor(type, texID));
	}
}

const int CGLShaderWarper::getTexturesCnt()const
{
	return textures->size();
}

void CGLShaderWarper::bindAllTextures()const
{
	for (unsigned int i = 0; i < textures->size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		if ((*textures)[i].type == STextureDescriptor::TEXD_ATLAS) CTextureAtlasManager::getAtlas((*textures)[i].texID)->getAtlasTexture()->bind();
		else glBindTexture(GL_TEXTURE_2D, (*textures)[i].texID);
	}
}
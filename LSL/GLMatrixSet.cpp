#include "GLMatrixSet.h"

glm::mat4 CGLMatrixSet::mIdentMatrix;

CGLMatrixSet::CGLMatrixSet()
{
	mMVPMatrix = glm::mat4(1.0f);
	mMVMatrix = glm::mat4(1.0f);
	mProjMatrix = glm::mat4(1.0f);
	mMMatrix = glm::mat4(1.0f);
	mVMatrix = glm::mat4(1.0f);
	mIdentMatrix = glm::mat4(1.0f);

	mvpPointer = &mMVPMatrix[0][0];
}


CGLMatrixSet::~CGLMatrixSet()
{

}

const float* CGLMatrixSet::getMVPfloatPointer()const
{
	return mvpPointer;
}

const float* CGLMatrixSet::getMVfloatPointer()const
{
	return  &mMVMatrix[0][0];
}

const float* CGLMatrixSet::getProjfloatPointer()const
{
	return  &mProjMatrix[0][0];
}
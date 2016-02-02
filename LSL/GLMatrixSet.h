#pragma once

#include "../../tools/includes/glm/glm.hpp"
#include "../../tools/includes/glm/gtc/matrix_transform.hpp"

class CGLMatrixSet
{
private:
	float* mvpPointer;
public:
	glm::mat4 mMVPMatrix;
	glm::mat4 mMVMatrix;
	glm::mat4 mProjMatrix;
	glm::mat4 mMMatrix;
	glm::mat4 mVMatrix;
	static glm::mat4 mIdentMatrix;
public:
	const float* getMVPfloatPointer()const;
	const float* getMVfloatPointer()const;
	const float* getProjfloatPointer()const;

	CGLMatrixSet();
	~CGLMatrixSet();
};


#include "VertexArray.h"
#include "global.h"

CVertexArray::CVertexArray(float* inputData,int inputDataLen)
{
	vertexCnt = 0;
	setData(inputData, inputDataLen);
}

CVertexArray::~CVertexArray()
{
	cleanup();
}

void CVertexArray::cleanup()
{
	if(data) delete[] data;
	data = NULL;
}

float* CVertexArray::getData()
{
	return data;
}

int  CVertexArray::getVertCnt()
{
	return vertexCnt;
}

void CVertexArray::setData(float* inputData, int inputDataLen)
{
	if (inputData == NULL) return;
	//int inputdataLen = sizeof(inputData) / sizeof(float);

	data = new float[inputDataLen];
	memcpy(data, inputData, inputDataLen*sizeof(float));

	vertexCnt = inputDataLen / 5;
}

void CVertexArray::drawArrays(int vertCnt)
{
	if (vertCnt > vertexCnt) return;

	glDrawArrays(GL_TRIANGLES, 0, vertCnt);
}

void CVertexArray::drawArrays()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, vertexCnt);
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

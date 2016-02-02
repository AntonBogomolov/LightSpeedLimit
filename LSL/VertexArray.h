#pragma once
class CVertexArray
{
private:
	float* data;
	int vertexCnt;
public:
	void cleanup();

	float* getData();
	int  getVertCnt();
	void setData(float* inputData,int inputDataLen);
	void drawArrays(int vertCnt);
	void drawArrays();

	CVertexArray(float* inputData,int inputDataLen);
	~CVertexArray();
};


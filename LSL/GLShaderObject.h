#pragma once
#include <string>

class CGLShaderObject
{
private:
	int m_hProgram;
	int m_hVertexShader;
	int m_hFragmentShader;

	int positionAttribHandle;
	int texCoodrAttribHandle;
	int normalsAttribHandle;
	int indexAttribHandle;

	int mvpMatrixUnifHandle;
	int mvMatrixUnifHandle;
	int pMatrixUnifHandle;

	int lColBuffHandle;
	int lPosBuffHandle;
	int lRadBuffHandle;
	int lCntfHandle;

	bool isUsing;
public:	
	const bool isValid()const;
	const bool getIsUsing()const;
	void setIsUsing(const bool isUsing);
	void cleanup();
	void enable()const;
	static void disable();
	void disableAttribArray();
	const int getUniformParameterID(const std::string pszParameter)const;
	const int getAttribParameterID(const std::string pszParameter)const;
	void setUniformParameter1i(const std::string pszParameter, const int n1)const;
	void setUniformParameter1f(const std::string pszParameter, const float p1)const;
	void setUniformParameter2f(const std::string pszParameter, const float p1, const float p2)const;
	void setUniformParameter3f(const std::string pszParameter, const float p1, const float p2, const float p3)const;
	void linkFloatBuffer(const std::string buffName, const float* buffer)const;
	void linkModelViewProjectionMatrix(const std::string matrixName, const float* maxtrix)const;
	void linkCamera(const std::string camName, const float xCamera, const float yCamera, const float zCamera)const;
	void setUniformParameter1i(const int idParameter, const int n1)const;
	void setUniformParameter1f(const int idParameter, const float p1)const;
	void setUniformParameter1fv(const int idParameter, const float* buff, const int cnt)const;
	void setUniformParameter3f(const int idParameter, const float p1, const float p2, const float p3)const;
	void setUniformParameter3fv(const int idParameter, const float* buff, const int cnt)const;
	void setUniformParameter2f(const int idParameter, const float p1, const float p2)const;
	void setUniformParameter2fv(const int idParameter, const float* buff, const int cnt)const;
	void linkFloatBuffer(const int buffId, const float* buffer)const;
	void linkModelViewProjectionMatrix(const int matrixId, const float* maxtrix)const;
	void linkModelViewProjectionMatrix(const float* maxtrix)const;
	void linkModelViewMatrix(const float* maxtrix)const;
	void linkProjectionMatrix(const float* maxtrix)const;
	void linkLightPosData(const float* posData, const int lCnt)const;
	void linkLightColData(const float* colData, const  int lCnt)const;
	void linkLightRadData(const float* radData, const int lCnt)const;
	void linkLightCnt(const int lCnt)const;
	void linkCamera(const int cameraId, const float xCamera, const float yCamera, const float zCamera)const;
	void linkPosAndTexCoordData(const float* data)const;
	void linkIndexData(const int* data)const;
	void linkPosData()const;
	void linkTexData()const;
	void linkNormData()const;
	void linkIndexData()const;

	bool init(const std::string pszVertexShader, const std::string pszFragmentShader);
	
	CGLShaderObject();
	~CGLShaderObject();
};


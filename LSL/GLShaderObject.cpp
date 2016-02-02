#include "GLShaderObject.h"
#include "global.h"
#include "Log.h"

#include "ResourceManager.h"

CGLShaderObject::CGLShaderObject()
{
	m_hProgram = 0;
	m_hVertexShader = 0;
	m_hFragmentShader = 0;

	positionAttribHandle = -1;
	texCoodrAttribHandle = -1;
	normalsAttribHandle = -1;
	indexAttribHandle = -1;

	mvpMatrixUnifHandle = -1;
	mvMatrixUnifHandle  = -1;
	pMatrixUnifHandle = -1;

	lColBuffHandle = -1;
	lPosBuffHandle = -1;
	lRadBuffHandle = -1;
	lCntfHandle = -1;

	isUsing = true;
}


CGLShaderObject::~CGLShaderObject()
{
	cleanup();
}

const bool CGLShaderObject::isValid()const
{
	return m_hProgram != 0;
}

void CGLShaderObject::cleanup()
{
	if (m_hFragmentShader != 0)
	{
		glDeleteShader(m_hFragmentShader);
		m_hFragmentShader = 0;
	}
	if (m_hVertexShader != 0)
	{
		glDeleteShader(m_hVertexShader);
		m_hVertexShader = 0;
	}
	if (m_hProgram != 0)
	{
		glDeleteProgram(m_hProgram);
		m_hProgram = 0;
	}
}

void CGLShaderObject::enable()const
{
	glUseProgram(m_hProgram);
}

void CGLShaderObject::disable()
{
	glUseProgram(0);	
}

void CGLShaderObject::disableAttribArray()
{
	positionAttribHandle = glGetAttribLocation(m_hProgram, "aPosition");
	if (positionAttribHandle != -1)glDisableVertexAttribArray(positionAttribHandle);
	texCoodrAttribHandle = glGetAttribLocation(m_hProgram, "aTextureCoord");
	if (texCoodrAttribHandle != -1)glDisableVertexAttribArray(texCoodrAttribHandle);
	normalsAttribHandle = glGetAttribLocation(m_hProgram, "aNormal");
	if (normalsAttribHandle != -1) glDisableVertexAttribArray(normalsAttribHandle);
	indexAttribHandle = glGetAttribLocation(m_hProgram, "aIndex");
	if (indexAttribHandle != -1) glDisableVertexAttribArray(indexAttribHandle);
}

const int CGLShaderObject::getUniformParameterID(const std::string pszParameter)const
{
	int id = glGetUniformLocation(m_hProgram, pszParameter.data());
	return id;
}

const int CGLShaderObject::getAttribParameterID(const std::string pszParameter)const
{
	int id = glGetAttribLocation(m_hProgram, pszParameter.data());
	return id;
}

void CGLShaderObject::setUniformParameter1i(const std::string pszParameter, const int n1)const
{
	int h = glGetUniformLocation(m_hProgram, pszParameter.data());
	glUniform1i(h, n1);
}

void CGLShaderObject::setUniformParameter1f(const std::string pszParameter, const float p1)const
{	
	glUniform1f(glGetUniformLocation(m_hProgram, pszParameter.c_str()), p1);
}

void CGLShaderObject::setUniformParameter2f(const std::string pszParameter, const float p1, const float p2)const
{
	glUniform2f(glGetUniformLocation(m_hProgram, pszParameter.data()), p1, p2);
}

void CGLShaderObject::setUniformParameter3f(const std::string pszParameter, const float p1, const float p2, const float p3)const
{
	glUniform3f(glGetUniformLocation(m_hProgram, pszParameter.data()), p1, p2, p3);
}

void CGLShaderObject::linkFloatBuffer(const std::string buffName, const float* buffer)const
{
	glUseProgram(m_hProgram);
	int buffId = glGetAttribLocation(m_hProgram, buffName.data());

	glEnableVertexAttribArray(buffId);
	glVertexAttribPointer(buffId, 3, GL_FLOAT, false, 0, buffer);
}

void CGLShaderObject::linkModelViewProjectionMatrix(const std::string matrixName, const float* maxtrix)const
{
	glUseProgram(m_hProgram);
	int matrixId = glGetUniformLocation(m_hProgram, matrixName.data());

	glUniformMatrix4fv(matrixId, 1, false, maxtrix);
}

void CGLShaderObject::linkCamera(const std::string camName, const float xCamera, const  float yCamera, const float zCamera)const
{
	glUseProgram(m_hProgram);
	int cameraId = glGetUniformLocation(m_hProgram, camName.data());

	glUniform3f(cameraId, xCamera, yCamera, zCamera);
}

void CGLShaderObject::setUniformParameter1i(const int idParameter, const int n1)const
{
	glUniform1i(idParameter, n1);
}

void CGLShaderObject::setUniformParameter1f(const int idParameter, const float p1)const
{
	glUniform1f(idParameter, p1);
}

void CGLShaderObject::setUniformParameter1fv(const int idParameter, const float* buff, const int cnt)const
{
	glUseProgram(m_hProgram);
	glUniform1fv(idParameter, cnt, buff);
}

void CGLShaderObject::setUniformParameter3f(const int idParameter, const float p1, const float p2, const  float p3)const
{
	glUniform3f(idParameter, p1, p2, p3);
}

void CGLShaderObject::setUniformParameter3fv(const int idParameter, const float* buff, const int cnt)const
{
	glUseProgram(m_hProgram);
	glUniform3fv(idParameter, cnt, buff);
}

void CGLShaderObject::setUniformParameter2f(const int idParameter, const float p1, const float p2)const
{
	glUniform2f(idParameter, p1, p2);
}

void CGLShaderObject::setUniformParameter2fv(const int idParameter, const float* buff, const int cnt)const
{
	glUseProgram(m_hProgram);
	glUniform2fv(idParameter, cnt, buff);
}

void CGLShaderObject::linkFloatBuffer(const int buffId, const float* buffer)const
{
	glUseProgram(m_hProgram);
	glEnableVertexAttribArray(buffId);
	glVertexAttribPointer(buffId, 3, GL_FLOAT, false, 0, buffer);
}

void CGLShaderObject::linkModelViewProjectionMatrix(const int matrixId, const float* maxtrix)const
{
	glUseProgram(m_hProgram);
	glUniformMatrix4fv(matrixId, 1, false, maxtrix);
}

void CGLShaderObject::linkModelViewProjectionMatrix(const float* maxtrix)const
{
	glUseProgram(m_hProgram);
	glUniformMatrix4fv(mvpMatrixUnifHandle, 1, false, maxtrix);
}

void CGLShaderObject::linkModelViewMatrix(const float* maxtrix)const
{
	glUseProgram(m_hProgram);
	glUniformMatrix4fv(mvMatrixUnifHandle, 1, false, maxtrix);
}

void CGLShaderObject::linkProjectionMatrix(const float* maxtrix)const
{
	glUseProgram(m_hProgram);
	glUniformMatrix4fv(pMatrixUnifHandle, 1, false, maxtrix);
}

void CGLShaderObject::linkLightPosData(const float* posData, const int lCnt)const
{
	if (lCnt == 0) return;
	glUseProgram(m_hProgram);
	glUniform2fv(lPosBuffHandle, lCnt, posData);
}

void CGLShaderObject::linkLightColData(const float* colData, const int lCnt)const
{
	if (lCnt == 0) return;
	glUseProgram(m_hProgram);
	glUniform3fv(lColBuffHandle, lCnt, colData);
}

void CGLShaderObject::linkLightRadData(const float* radData, const int lCnt)const
{
	if (lCnt == 0) return;
	glUseProgram(m_hProgram);
	glUniform1fv(lRadBuffHandle, lCnt, radData);
}

void CGLShaderObject::linkLightCnt(const int lCnt)const
{
	glUseProgram(m_hProgram);
	glUniform1i(lCntfHandle, lCnt);
}

void CGLShaderObject::linkCamera(const int cameraId, const float xCamera, const float yCamera, const float zCamera)const
{
	glUseProgram(m_hProgram);
	glUniform3f(cameraId, xCamera, yCamera, zCamera);
}

void CGLShaderObject::linkPosAndTexCoordData(const float* data)const
{
	if (positionAttribHandle == -1 || texCoodrAttribHandle == -1) return;

	glUseProgram(m_hProgram);

	glEnableVertexAttribArray(positionAttribHandle);	
	glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, false, 20, data);

	data = data + 3;
	glEnableVertexAttribArray(texCoodrAttribHandle);
	glVertexAttribPointer(texCoodrAttribHandle, 2, GL_FLOAT, false, 20, data);

	data = data - 3;	
}

void CGLShaderObject::linkIndexData(const int* data)const
{
	if (indexAttribHandle == -1) return;

	glUseProgram(m_hProgram);

	glEnableVertexAttribArray(indexAttribHandle);
	glVertexAttribPointer(indexAttribHandle, 1, GL_INT, false, 0, data);
//	glDisableVertexAttribArray(indexAttribHandle);
}

void CGLShaderObject::linkIndexData()const
{
	if (indexAttribHandle == -1) return;

	glUseProgram(m_hProgram);
	glEnableVertexAttribArray(indexAttribHandle);
	glVertexAttribPointer(indexAttribHandle, 1, GL_INT, false, 0, 0);
//	glDisableVertexAttribArray(indexAttribHandle);
}

void CGLShaderObject::linkPosData()const
{
	glUseProgram(m_hProgram);
	glEnableVertexAttribArray(positionAttribHandle);
	glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, false, 0, 0);
//	glDisableVertexAttribArray(positionAttribHandle);
}

void CGLShaderObject::linkTexData()const
{
	glUseProgram(m_hProgram);
	glEnableVertexAttribArray(texCoodrAttribHandle);
	glVertexAttribPointer(texCoodrAttribHandle, 2, GL_FLOAT, false, 0, 0);
//	glDisableVertexAttribArray(texCoodrAttribHandle);
}

void CGLShaderObject::linkNormData()const
{
	glUseProgram(m_hProgram);
	glEnableVertexAttribArray(normalsAttribHandle);
	glVertexAttribPointer(normalsAttribHandle, 3, GL_FLOAT, false, 0, 0);
//	glDisableVertexAttribArray(normalsAttribHandle);
}

bool CGLShaderObject::init(const std::string pszVertexShader, const std::string pszFragmentShader)
{
	cleanup();

	m_hProgram			= glCreateProgram();
	m_hVertexShader		= glCreateShader(GL_VERTEX_SHADER);
	m_hFragmentShader	= glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertexText = CResourceManager::loadTextFile(pszVertexShader);

	const char *strFileData = vertexText.c_str();
	GLint nBytes = (GLint)strlen(strFileData);
	glShaderSource(m_hVertexShader, 1, &strFileData, &nBytes);
	glCompileShader(m_hVertexShader);	
	GLint compiled;
	glGetShaderiv(m_hVertexShader, GL_COMPILE_STATUS, &compiled);
	
	if (compiled == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(m_hVertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(m_hVertexShader, infoLogLength, NULL, strInfoLog);
		
		//fprintf(stderr, "Compile failure in %s shader:\n%s\n", "geometry", strInfoLog);
		CLog::getInstance()->addError(std::string("CGLShaderObject::init() \t| Shader compile failure: ") + pszVertexShader + " " + strInfoLog);
		delete[] strInfoLog;
		
		glDeleteShader(m_hVertexShader);
		m_hVertexShader = 0;
		return false;
	}	

	std::string fragText = CResourceManager::loadTextFile(pszFragmentShader);

	strFileData = fragText.c_str();
	nBytes = (GLint)vertexText.length();
	glShaderSource(m_hFragmentShader, 1, &strFileData, NULL);
	glCompileShader(m_hFragmentShader);	
	glGetShaderiv(m_hFragmentShader, GL_COMPILE_STATUS, &compiled);
	if (compiled == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(m_hVertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(m_hVertexShader, infoLogLength, NULL, strInfoLog);

		//fprintf(stderr, "Compile failure in %s shader:\n%s\n", "fargment", strInfoLog);
		CLog::getInstance()->addError(std::string("CGLShaderObject::init() \t| Shader compile failure: ") + pszFragmentShader + " " + strInfoLog);
		delete[] strInfoLog;

		glDeleteShader(m_hFragmentShader);
		m_hVertexShader = 0;
		return false;
	}

	glAttachShader(m_hProgram, m_hVertexShader);
	glAttachShader(m_hProgram, m_hFragmentShader);
	glLinkProgram(m_hProgram);
	GLint* linkStatus = new GLint[1];
	glGetProgramiv(m_hProgram, GL_LINK_STATUS, linkStatus);
	if (linkStatus[0] != GL_TRUE)
	{
		CLog::getInstance()->addError("CGLShaderObject::init() \t| Shader link failure");
		glDeleteProgram(m_hProgram);
		m_hProgram = 0;
		return false;
	}
	delete[] linkStatus;

	positionAttribHandle = -1;
	texCoodrAttribHandle = -1;	
	normalsAttribHandle = -1;
	indexAttribHandle = -1;

	mvpMatrixUnifHandle = -1;
	mvMatrixUnifHandle  = -1;
	pMatrixUnifHandle   = -1;

	positionAttribHandle = glGetAttribLocation(m_hProgram, "aPosition");
	if (positionAttribHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for aPosition");
	}

	texCoodrAttribHandle = glGetAttribLocation(m_hProgram, "aTextureCoord");
	if (texCoodrAttribHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for aTextureCoord");
	}

	normalsAttribHandle = glGetAttribLocation(m_hProgram, "aNormal");
	if (normalsAttribHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for aTextureCoord");
	}

	indexAttribHandle = glGetAttribLocation(m_hProgram, "aIndex");
	if (indexAttribHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for aTextureCoord");
	}

	mvpMatrixUnifHandle = glGetUniformLocation(m_hProgram, "uMVPMatrix");
	if (mvpMatrixUnifHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for uMVPMatrix");
	}

	mvMatrixUnifHandle = glGetUniformLocation(m_hProgram, "uMVMatrix");
	if (mvMatrixUnifHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for uMVPMatrix");
	}

	pMatrixUnifHandle = glGetUniformLocation(m_hProgram, "uPMatrix");
	if (pMatrixUnifHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for uMVPMatrix");
	}

	lColBuffHandle = -1;
	lPosBuffHandle = -1;
	lRadBuffHandle = -1;
	lCntfHandle = -1;

	lPosBuffHandle = glGetUniformLocation(m_hProgram, "lPos");
	if (lPosBuffHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for aPosition");
	}

	lColBuffHandle = glGetUniformLocation(m_hProgram, "lCol");
	if (lColBuffHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for aTextureCoord");
	}

	lRadBuffHandle = glGetUniformLocation(m_hProgram, "lRad");
	if (lRadBuffHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for aTextureCoord");
	}

	lCntfHandle = glGetUniformLocation(m_hProgram, "light_source_cnt");
	if (lCntfHandle == -1)
	{
		//throw new RuntimeException("Could not get attrib location for uMVPMatrix");
	}
	
	CLog::getInstance()->addInfo(std::string("CGLShaderObject::init() \t| Shader compile success: ") + pszFragmentShader);
	return true;
}

const bool CGLShaderObject::getIsUsing()const
{
	return isUsing;
}

void CGLShaderObject::setIsUsing(const bool isUsing)
{
	this->isUsing = isUsing;
}
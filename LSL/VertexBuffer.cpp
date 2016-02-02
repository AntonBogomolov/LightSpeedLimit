#include "VertexBuffer.h"
#include "global.h"
#include "GLShaderObject.h"
#include "SyncHelper.h"

CVertexBuffer::CVertexBuffer(int mode, int vertCoordCnt, int texCoordCnt)
{
	_vert_id = 0;
	_norm_id = 0;
	_tex_id = 0;
	_tan_id = 0;
	_ind_id = 0;
	_vert_len = 0;
	_norm_len = 0;
	_tex_len = 0;
	_tex2_len = 0;
	_tan_len = 0;
	_ind_len = 0;

	_vert_coord_cnt = vertCoordCnt;
	_tex_coord_cnt  = texCoordCnt;
		
	posData = NULL;
	texData = NULL;
	tex2Data = NULL;
	normData = NULL;
	indData = NULL;

	posDataLen = 0;
	texDataLen = 0;
	tex2DataLen = 0;
	normDataLen = 0;
	indDataLen = 0;
	
	isVertAlloc = false;
	isTexAlloc = false;
	isTex2Alloc = false;
	isNormAlloc = false;
	isIndAlloc = false;

	isVertUpdated = false;
	isTexUpdated = false;
	isTex2Updated = false;
	isNormUpdated = false;
	isIndUpdated = false;

	_do_binding = false;
	this->mode = mode;
}

CVertexBuffer::CVertexBuffer(const int maxQuads, int mode, int vertCoordCnt, int texCoordCnt)
{
	_vert_id = 0;
	_norm_id = 0;
	_tex_id = 0;
	_tan_id = 0;
	_ind_id = 0;
	_vert_len = 0;
	_norm_len = 0;
	_tex_len = 0;
	_tex2_len = 0;
	_tan_len = 0;
	_ind_len = 0;

	_vert_coord_cnt = vertCoordCnt;
	_tex_coord_cnt = texCoordCnt;

	posDataLen = maxQuads  * _vert_coord_cnt; //* sizeof(int);
	texDataLen = maxQuads  * _tex_coord_cnt;  //* sizeof(int);
	tex2DataLen = maxQuads * _tex_coord_cnt;  //* sizeof(int);
	normDataLen = 0;
	indDataLen = maxQuads  * _vert_coord_cnt; //*sizeof(short);
	
	posData = new float[posDataLen]();
	texData = new float[texDataLen]();
	tex2Data = new float[tex2DataLen]();
	indData = new short[indDataLen]();
	
	isVertAlloc = false;
	isTexAlloc = false;
	isTex2Alloc = false;
	isNormAlloc = false;
	isIndAlloc = false;

	isVertUpdated = false;
	isTexUpdated = false;
	isTex2Updated = false;
	isNormUpdated = false;
	isIndUpdated = false;

	_do_binding = false;
	this->mode = mode;
}

CVertexBuffer::~CVertexBuffer()
{
	cleanup();
}

void CVertexBuffer::cleanup()
{
	clearData();

	GLuint tmp = 0;
	if (glIsBuffer(_vert_id))
	{		
		glDeleteBuffers(1, &tmp);
	}

	if (glIsBuffer(_norm_id))
	{
		glDeleteBuffers(1, &tmp);
	}

	if (glIsBuffer(_tex_id))
	{
		glDeleteBuffers(1, &tmp);
	}

	if (glIsBuffer(_tan_id))
	{
		glDeleteBuffers(1, &tmp);
	}

	if (glIsBuffer(_ind_id))
	{
		glDeleteBuffers(1, &tmp);
	}

	isVertAlloc = false;
	isTexAlloc = false;
	isTex2Alloc = false;
	isNormAlloc = false;
	isIndAlloc = false;

	isVertUpdated = false;
	isTexUpdated = false;
	isTex2Updated = false;
	isNormUpdated = false;
	isIndUpdated = false;
}

void CVertexBuffer::setMode(const int mode)
{
	this->mode = mode;
}

int CVertexBuffer::getMode() const
{
	return mode;
}

const float* CVertexBuffer::getVertData()const
{
	return posData;
}
const float* CVertexBuffer::getTextData()const
{
	return texData;
}
const float* CVertexBuffer::getAdditionalTextData()const
{
	return tex2Data;
}
const float* CVertexBuffer::getNormtData() const
{
	return normData;
}
const short* CVertexBuffer::getIndData()const
{
	return indData;
}

float* CVertexBuffer::getVertDataForModify()const
{
	isVertUpdated = false;
	return posData;	
}
float* CVertexBuffer::getTextDataForModify()const
{
	isTexUpdated = false;
	return texData;	
}
float* CVertexBuffer::getAdditionalTextDataForModify()const
{
	isTex2Updated = false;
	return tex2Data;	
}
float* CVertexBuffer::getNormtDataForModify() const
{
	isNormUpdated = false;
	return normData;	
}
short* CVertexBuffer::getIndDataForModify()const
{
	isIndUpdated = false;
	return indData;	
}

const int CVertexBuffer::getVertDataLen()const
{
	return posDataLen;
}
const int CVertexBuffer::getTextDataLen()const
{
	return texDataLen;
}
const int CVertexBuffer::getAdditionalTextDataLen()const
{
	return tex2DataLen;
}
const int CVertexBuffer::getNormtDataLen()const
{
	return normDataLen;
}
const int CVertexBuffer::getIndDataLen()const
{
	return indDataLen;
}

void CVertexBuffer::clearData()
{
	if (posData != NULL)
	{
		delete[] posData;
		posData = NULL;
	}
	if (texData != NULL)
	{
		delete[] texData;
		texData = NULL;
	}
	if (tex2Data != NULL)
	{
		delete[] tex2Data;
		tex2Data = NULL;
	}
	if (normData != NULL)
	{
		delete[] normData;
		normData = NULL;
	}
	if (indData != NULL)
	{
		delete[] indData;
		indData = NULL;
	}
	
	posDataLen = 0;
	texDataLen = 0;
	tex2DataLen = 0;
	normDataLen = 0;
	indDataLen = 0;
}

void CVertexBuffer::setVerticesData(const float* data, const int len)
{
	if (len <= 0 || data == NULL) return;

	if (posData == NULL || posDataLen < len)
	{
		delete[] posData;
		posData = new float[len];
	}
	posDataLen = len;
		
	memcpy(posData, data, posDataLen*sizeof(float));
	
	_vert_len = len / _vert_coord_cnt;

	isVertAlloc = false;
	isVertUpdated = false;
}

void CVertexBuffer::setVertices(const int usage) const
{
	GLuint tmp;
	if (!glIsBuffer(_vert_id))
	{		
		glGenBuffers(1, &tmp);
		_vert_id = tmp;	
	}
	if (posData == NULL || _vert_len == 0)
	{		
		return;
	}
	
	int count = _vert_len;

	glBindBuffer(GL_ARRAY_BUFFER, _vert_id);
	int size = (count * _vert_coord_cnt) * sizeof(int);
	if (!isVertAlloc)
	{
		glBufferData(GL_ARRAY_BUFFER, size, posData, usage);
		isVertAlloc = true;
	}
	else glBufferSubData(GL_ARRAY_BUFFER, 0, size, posData);
	isVertUpdated = true;
}

void CVertexBuffer::setNormalsData(const float* data, const int len)
{	
	if (len <= 0 || data == NULL) return;

	if (normData == NULL || normDataLen < len)
	{
		delete[] normData;
		normData = new float[len];
	}
	normDataLen = len;
	memcpy(normData, data, normDataLen*sizeof(float));

	_norm_len = len / _vert_coord_cnt;

	isNormUpdated = false;
	isNormAlloc = false;
}

void CVertexBuffer::setNormals(const int usage) const
{
	GLuint tmp;
	if (!glIsBuffer(_norm_id))
	{		
		glGenBuffers(1, &tmp);
		_norm_id = tmp;
	}
	if (normData == NULL || _norm_len == 0)
	{
		return;
	}	
	int count = _norm_len;

	glBindBuffer(GL_ARRAY_BUFFER, _norm_id);
	int size = (count * _vert_coord_cnt) * sizeof(int);
	if (!isNormAlloc)
	{
		glBufferData(GL_ARRAY_BUFFER, size, normData, usage);
		isNormAlloc = true;
	}
	else glBufferSubData(GL_ARRAY_BUFFER, 0, size, normData);
	isNormUpdated = true;
}

void CVertexBuffer::setTexData(const float* data, const int len)
{
	if (len <= 0 || data == NULL) return;

	if (texData == NULL || texDataLen < len)
	{
		delete[] texData;
		texData = new float[len];
	}
	texDataLen = len;
	memcpy(texData, data, texDataLen*sizeof(float));
		
	_tex_len = len / _tex_coord_cnt;
	isTexAlloc = false;
	isTexUpdated = false;
}

void CVertexBuffer::setAdditionalTexData(const float* data, const int len)
{
	if (len <= 0 || data == NULL) return;

	if (tex2Data == NULL || tex2DataLen < len)
	{
		delete[] tex2Data;
		tex2Data = new float[len];
	}
	tex2DataLen = len;
	memcpy(tex2Data, data, tex2DataLen*sizeof(float));
	
	_tex2_len = len / _tex_coord_cnt;
	isTex2Alloc = false;
	isTex2Updated = false;
}

void CVertexBuffer::setTexCoords(const int usage) const
{
	GLuint tmp;
	if (!glIsBuffer(_tex_id))
	{		
		glGenBuffers(1, &tmp);
		_tex_id = tmp;
	}

	if (texData == NULL || _tex_len == 0)
	{
		return;
	}	
	int count = _tex_len;

	glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
	int size = (count * _tex_coord_cnt) * sizeof(int);
	if (!isTexAlloc)
	{
		glBufferData(GL_ARRAY_BUFFER, size, texData, usage);
		isTexAlloc = true;
	}
	else glBufferSubData(GL_ARRAY_BUFFER, 0, size, texData);
	isTexUpdated = true;
}

void CVertexBuffer::useDefaultTexCoord(const int usage) const
{
	setTexCoords(usage);
}

void CVertexBuffer::useAdditionalTexCoord(const int usage) const
{
	GLuint tmp;
	if (!glIsBuffer(_tex_id))
	{		
		glGenBuffers(1, &tmp);
		_tex_id = tmp;
	}

	if (tex2Data == NULL || _tex2_len == 0)
	{
		return;
	}
	
	int count = _tex2_len;
	glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
	int size = (count * _tex_coord_cnt) * sizeof(int);
	if (!isTex2Alloc)
	{
		glBufferData(GL_ARRAY_BUFFER, size, tex2Data, usage);
		isTex2Alloc = true;
	}
	else glBufferSubData(GL_ARRAY_BUFFER, 0, size, tex2Data);
	isTex2Updated = true;
}

void CVertexBuffer::setTangents(const int count, const byte* data, const int usage)
{
	bool lockResult = CSyncHelper::getInstance()->getDrawMutex()->try_lock();

	GLuint tmp;
	if (!glIsBuffer(_tan_id))
	{		
		glGenBuffers(1, &tmp);
		_tan_id = tmp;
	}

	glBindBuffer(GL_ARRAY_BUFFER, _tan_id);
	glBufferData(GL_ARRAY_BUFFER, (count * 4) * sizeof(int), data, usage);
	_tan_len = count;

	if (lockResult) CSyncHelper::getInstance()->getDrawMutex()->unlock();
}

void CVertexBuffer::setIndicesData(const short* data, const int len)
{
	if (len <= 0 || data == NULL) return;

	if (indData == NULL || indDataLen < len)
	{
		delete[] indData;
		indData = new short[len];		
	}
	indDataLen = len;
	memcpy(indData, data, indDataLen*sizeof(short));

	_ind_len = len;
	isIndAlloc = false;
	isIndUpdated = false;
}

void CVertexBuffer::setIndices(const int usage) const
{
	GLuint tmp;
	if (!glIsBuffer(_ind_id))
	{		
		glGenBuffers(1, &tmp);
		_ind_id = tmp;
		
	}
	if (indData == NULL || _ind_len == 0)
	{	
		return;
	}
	
	int count = _ind_len;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ind_id);
	int size = count * sizeof(short);
	if (!isIndAlloc)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indData, usage);
		isIndAlloc = true;
	}
	else glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indData);
	isIndUpdated = true;	
}

void CVertexBuffer::setBinding(const bool do_binding)
{
	_do_binding = do_binding;
}

void CVertexBuffer::bind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, _vert_id);
	if (_norm_len > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _norm_id);
	}
	if (_tex_len > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ind_id);
}

void CVertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
//	glEnableVertexAttribArray(0);
	glDisableVertexAttribArray(0);
}

void CVertexBuffer::drawArrays(const int count)const
{
	int cnt = count;
	if (count == 0)
	{
		cnt = _vert_len;
	}
	else
	if (count > _vert_len)
	{
		return;
	}

	if (_do_binding)
	{
		bind();
	}

	glDrawArrays(mode, 0, cnt);
}

void CVertexBuffer::drawArrays(const int first, const int count)const
{
	if (_do_binding)
	{
		bind();
	}

	glDrawArrays(mode, first, count);
}

void CVertexBuffer::drawElements(const int count)const
{
	int cnt = count;
	if (count == 0)
	{
		cnt = _ind_len;
	}

	if (_do_binding)
	{
		bind();
	}

	glDrawElements(mode, cnt, GL_UNSIGNED_SHORT, 0);
}

void CVertexBuffer::drawElements(const int count, const CGLShaderObject* shader)const
{
	int cnt = count;
	if (count == 0)
	{
		cnt = _ind_len;
	}

	if (_vert_len == 0)
	{
		return;
	}
	
	if (!isVertUpdated) setVertices(GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, _vert_id);
	shader->linkPosData();

	if (_norm_len > 0)
	{
		if (!isNormUpdated) setNormals(GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _norm_id);
		shader->linkNormData();
	}
	if (_tex_len > 0)
	{
		if (!isTexUpdated) setTexCoords(GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, _tex_id);
		shader->linkTexData();
	}	
	if (!isIndUpdated) setIndices(GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ind_id);

	glDrawElements(mode, cnt, GL_UNSIGNED_SHORT, 0);
	unbind();
}
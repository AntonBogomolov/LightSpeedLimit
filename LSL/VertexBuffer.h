#pragma once
#define byte unsigned char

class CGLShaderObject;

class CVertexBuffer
{
private:
	mutable int _vert_id;
	mutable int _norm_id;
	mutable int _tex_id;
	mutable int _tan_id;
	mutable int _ind_id;

	int _vert_len;
	int _norm_len;
	int _tex_len;
	int _tex2_len;
	int _tan_len;
	int _ind_len;

	int _vert_coord_cnt;
	int _tex_coord_cnt;

	float* posData;
	float* texData;
	float* tex2Data;
	float* normData;
	short* indData;
	
	int posDataLen;
	int texDataLen;
	int tex2DataLen;
	int normDataLen;
	int indDataLen;

	mutable bool isVertAlloc;
	mutable bool isTexAlloc;
	mutable bool isTex2Alloc;
	mutable bool isNormAlloc;
	mutable bool isIndAlloc;

	mutable bool isVertUpdated;
	mutable bool isTexUpdated;
	mutable bool isTex2Updated;
	mutable bool isNormUpdated;
	mutable bool isIndUpdated;

	bool _do_binding;
	int mode;
private:
	void setVertices(const int usage) const;
	void setNormals(const int usage) const;
	void setTexCoords(const int usage) const;
	void useDefaultTexCoord(const int usage) const;
	void useAdditionalTexCoord(const int usage) const;
	void setTangents(const int count, const byte* data, const int usage);
	void setIndices(const int usage) const;
public:
	void cleanup();
	const float* getVertData()const;
	const float* getTextData()const;
	const float* getAdditionalTextData()const;
	const float* getNormtData()const;
	const short* getIndData()const;

	float* getVertDataForModify()const;
	float* getTextDataForModify()const;
	float* getAdditionalTextDataForModify()const;
	float* getNormtDataForModify()const;
	short* getIndDataForModify()const;

	const int getVertDataLen()const;
	const int getTextDataLen()const;
	const int getAdditionalTextDataLen()const;
	const int getNormtDataLen()const;
	const int getIndDataLen()const;
	
	void clearData();
	void setMode(const int mode);
	int  getMode() const;

	void setVerticesData(const float* data, const int len);	
	void setNormalsData(const float* data, const  int len);
	void setTexData(const float* data, const int len);
	void setAdditionalTexData(const float* data, const int len);	
	void setIndicesData(const short* data, const int len);
	
	void setBinding(const bool do_binding);
	void bind()const;
	static void unbind();

	void drawArrays(const int count)const;
	void drawArrays(const int first, const int count)const;
	void drawElements(const int count)const;
	void drawElements(const int count, const CGLShaderObject* shader)const;

	CVertexBuffer(int mode = 4, int vertCoordCnt = 3, int texCoordCnt = 2);
	CVertexBuffer(const int maxQuads, int mode = 4, int vertCoordCnt = 3, int texCoordCnt = 2);
	~CVertexBuffer();
};


#pragma once

#include <vector>
#include "../../tools/includes/SDL.h"
#include "../../tools/includes/GL/glew.h"

class CFrameBuffer
{
protected:
	int _width;
	int _height;
	GLuint _frame_id;
	GLuint _depth_id;
	GLuint _stencil_id;
	std::vector<GLuint>* _tex_id;
	int* _buffers;

	int _max_color_attachments;
public:
	void cleanup();
	static void clearScreen();
	void getWidthHeight(int &width, int &height)const;
	void bindInput()const;
	void bindInput(const int num)const;
	const int getTexId()const;
	void bindOutput()const;
	void bindOutput(const int num)const;
	void bindTex(const int num)const;
	void bindTex()const;
	void bindLastTex()const;
	void setSize(const int width, const int height);
	void check();
	void blitTo(const CFrameBuffer dest, const int mask, const int filter);
	void attachRender(const int iformat);
	void attachTexture(const int iformat, const int filter);
	void deattachTextures();

	static void unbind();

	CFrameBuffer();
	CFrameBuffer(const int width, const int height);
	~CFrameBuffer();
};


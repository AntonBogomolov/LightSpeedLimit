#pragma once
#include <string>

#include "../../tools/includes/GL/glew.h"
#include "../../tools/includes/SDL.h"

/// CTexture class
class CTexture
{
private:
	int w;
	int h;
	int bpp;

	GLuint _id;
	void Load(const std::string filename);
	void Load(const SDL_Surface *image);
	void Load(const GLenum iformat, const int width, const int height, const GLfloat *pixels);
	void Load(const GLenum iformat, const int width, const int height, const GLubyte *pixels);
public:
	CTexture();
	CTexture(const std::string filename);
//	CTexture(const std::string* filenames);
	CTexture(const SDL_Surface *surface);
	CTexture(const GLenum iformat, const int width, const int height, const GLfloat *pixels);
	CTexture(const GLenum iformat, const int width, const int height, const GLubyte *pixels);
	~CTexture();

	void SetFiltering(const GLenum filter);
	void bind(int mode = GL_TEXTURE_2D)const;
	static void unbind();

	const int getW()const;
	const int getH()const;

	const GLuint getID()const;
};

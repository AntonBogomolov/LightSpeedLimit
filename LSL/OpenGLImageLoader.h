#pragma once

#include "Log.h"

#include <../../MyGUI/include/OpenGLPlatform/include/MyGUI_OpenGLImageLoader.h>
#include <string>

class COpenGLImageLoader : public MyGUI::OpenGLImageLoader
{
public:

	COpenGLImageLoader()
	{
	}

	virtual ~COpenGLImageLoader()
	{
	}

	virtual void* loadImage(int& _width, int& _height, MyGUI::PixelFormat& _format, const std::string& _filename)
	{
		std::string fullname = MyGUI::OpenGLDataManager::getInstance().getDataPath(_filename);
		unsigned char* result = 0;

		Uint8 bpp;
		GLenum iformat;
		
		SDL_Surface  *tex;
		if (!(tex = IMG_Load(fullname.c_str())))
		{
			MYGUI_LOG(Error, "loadImage - loading error");
			CLog::getInstance()->addError("COpenGLImageLoader::loadImage() \t| loading error");			
			return NULL;
		}

		bpp = tex->format->BitsPerPixel;
		if (bpp == 32) {
			iformat = GL_RGBA8;
			if (tex->format->Rmask == 0x000000ff) {
				_format = MyGUI::PixelFormat::R8G8B8A8;
			}
			else {
				_format = MyGUI::PixelFormat::Unknow;
			}
		}
		else if (bpp == 24) {
			iformat = GL_RGB8;
			if (tex->format->Rmask == 0x000000ff) {
				_format = MyGUI::PixelFormat::R8G8B8;
			}
			else {
				_format = MyGUI::PixelFormat::Unknow;
			}
		}
		else {
			MYGUI_LOG(Error, "loadImage - Not a truecolor image");
			CLog::getInstance()->addError("COpenGLImageLoader::loadImage() \t| loading error");
		}	

		_width = tex->w;
		_height = tex->h;
		size_t  sz = tex->w * tex->h * tex->format->BytesPerPixel;

		result = new unsigned char[sz];
				
		SDL_LockSurface(tex);
		bpp = tex->format->BytesPerPixel;
		if (bpp == 4)
		{
			for (unsigned int i = 0; i < (unsigned int)(tex->h * tex->w * bpp); i += bpp)
			{
				result[i + 0] = ((unsigned char*)tex->pixels)[i + 2];
				result[i + 1] = ((unsigned char*)tex->pixels)[i + 1];
				result[i + 2] = ((unsigned char*)tex->pixels)[i + 0];
				result[i + 3] = ((unsigned char*)tex->pixels)[i + 3];
			}
		}else
		if (bpp == 3)
		{
			for (unsigned int i = 0; i < (unsigned int)(tex->h * tex->w * bpp); i += bpp)
			{
				result[i + 0] = ((unsigned char*)tex->pixels)[i + 2];
				result[i + 1] = ((unsigned char*)tex->pixels)[i + 1];
				result[i + 2] = ((unsigned char*)tex->pixels)[i + 0];				
			}
		}
		else
		{
			memcpy(result, tex->pixels, sz);
		}

		SDL_UnlockSurface(tex);
		SDL_FreeSurface(tex);
		
		return result;	
	}

	virtual void saveImage(int _width, int _height, MyGUI::PixelFormat _format, void* _texture, const std::string& _filename)
	{
		int format;
		int bpp;

		if (_format == MyGUI::PixelFormat::R8G8B8A8)
		{
			bpp = 4;
			format = GL_RGBA8;
		}
		else if (_format == MyGUI::PixelFormat::R8G8B8)
		{
			bpp = 3;
			format = GL_RGB8;
		}
		else
		{
			MYGUI_LOG(Error, "Unsuitable texture format for saving.");
			return;
		}

		Uint32 rmask, gmask, bmask, amask;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				rmask = 0xff000000;
				gmask = 0x00ff0000;
				bmask = 0x0000ff00;
				amask = 0x000000ff;
		#else
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = 0xff000000;
		#endif

		SDL_Surface* image = SDL_CreateRGBSurfaceFrom(_texture, _width, _height, 8, _width, rmask, gmask, bmask, amask);
		IMG_SavePNG(image, _filename.c_str());
		SDL_FreeSurface(image);
	}
};


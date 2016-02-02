#include "FrameBuffer.h"
#include "global.h" 
#include "Log.h"

#define byte unsigned char

CFrameBuffer::CFrameBuffer()
{
	_width = 0;
	_height = 0;
	_frame_id = 0;
	_depth_id = 0;
	_stencil_id = 0;
	_buffers = NULL;
	_max_color_attachments = 7;

	_buffers = new int[_max_color_attachments];

	_tex_id = new std::vector<GLuint>();

	GLuint* frBuffId = new GLuint[1];
	glGenFramebuffers(1, frBuffId);
	_frame_id = frBuffId[0];
}

CFrameBuffer::CFrameBuffer(const int width, const int height)
{
	_frame_id = 0;
	_depth_id = 0;
	_stencil_id = 0;
	_buffers = NULL;
	_max_color_attachments = 7;

	_buffers = new int[_max_color_attachments];

	_tex_id = new std::vector<GLuint>();

	GLuint* frBuffId = new GLuint[1];
	glGenFramebuffers(1, frBuffId);
	_frame_id = frBuffId[0];

	_width = width;
	_height = height;
}


CFrameBuffer::~CFrameBuffer()
{
	cleanup();
}

void CFrameBuffer::cleanup()
{
	if (_tex_id != NULL)
	{
		for each (GLuint tex_id in (*_tex_id))
		{
			GLuint* texId = new GLuint[1];
			texId[0] = tex_id;
			glDeleteTextures(1, texId);
			delete[] texId;
		}
		delete _tex_id;
	}
	if (_depth_id != 0)
	{
		GLuint* dId = new GLuint[1];
		dId[0] = _depth_id;
		glDeleteRenderbuffers(1, dId);
		delete[] dId;
	}
	if (_stencil_id != 0)
	{
		GLuint* sId = new GLuint[1];
		sId[0] = _stencil_id;
		glDeleteRenderbuffers(1, sId);
		delete[] sId;
	}

	GLuint* fId = new GLuint[1];
	fId[0] = _frame_id;
	glDeleteFramebuffers(1, fId);
	delete[] fId;

	_buffers = NULL;
	_tex_id = NULL;
}

void CFrameBuffer::getWidthHeight(int &width, int &height)const
{
	width = _width;
	height = _height;
}

void CFrameBuffer::bindInput()const
{
	for (unsigned int i = 0; i < _tex_id->size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);		
		glBindTexture(GL_TEXTURE_2D, (*_tex_id)[i]);
	}	
}

void CFrameBuffer::bindInput(const int num)const
{
	if ((unsigned)(num + 1) > _tex_id->size())
	{
		return;
	}
	glBindTexture(GL_TEXTURE_2D, (*_tex_id)[num]);
}

const int CFrameBuffer::getTexId()const
{
	return  (*_tex_id)[0];
}

void CFrameBuffer::bindOutput()const
{
	if (_tex_id->size() == 0)
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _frame_id);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	if (_tex_id->size() == 1)
	{
		// glDrawBuffer(_buffers[0]);
	}
	else
	{
		// glDrawBuffers(_tex_id.size(), _buffers);
	}
}

void CFrameBuffer::bindOutput(const int num)const
{
	if ((unsigned)(num + 1) > _tex_id->size())
	{
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, _frame_id);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void CFrameBuffer::clearScreen()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glDrawBuffer(GL_BACK);
}

void CFrameBuffer::bindTex(const int num)const
{
	bindInput(num);
}

void CFrameBuffer::bindTex()const
{
	bindInput(0);
}

void CFrameBuffer::bindLastTex()const
{
	int last = _tex_id->size() - 1;
	bindInput(last);
}

void CFrameBuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// glDrawBuffer(GL_BACK);
}

void CFrameBuffer::setSize(const int width, const int height)
{
	_width = width;
	_height = height;
};

void CFrameBuffer::check()
{
	int status;

	glBindFramebuffer(GL_FRAMEBUFFER, _frame_id);
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		CLog::getInstance()->addError("CFrameBuffer::check() \t| FrameBuffer error");
	}
}

void CFrameBuffer::blitTo(const CFrameBuffer dest, const int mask, const int filter)
{
	// empty
}

void CFrameBuffer::attachRender(const int iformat)
{
	int attachment;
	GLuint* render_id = new GLuint[1];

	if (_width == 0 || _height == 0)
	{
		return;
	}

	if (iformat == GL_DEPTH_COMPONENT16 || iformat == GL_DEPTH_COMPONENT)
	{
		attachment = GL_DEPTH_ATTACHMENT;
	}
	else	
	{
		return;
	}

	glGenRenderbuffers(1, render_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _frame_id);
	glBindRenderbuffer(GL_RENDERBUFFER, render_id[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, iformat, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, render_id[0]);

	if (attachment == GL_DEPTH_ATTACHMENT)
	{
		_depth_id = render_id[0];
	}
	else
	if (attachment == GL_STENCIL_ATTACHMENT)
	{
		_stencil_id = render_id[0];
	}
}

void CFrameBuffer::attachTexture(const int iformat, const int filter)
{
	int format;
	int type;
	int attachment;
	int filt = filter;
	GLuint* tex_id = new GLuint[1];

	if (_width == 0 || _height == 0)
	{
		return;
	}

	if (_tex_id->size() == _max_color_attachments)
	{
		return;
	}

	attachment = GL_COLOR_ATTACHMENT0 + _tex_id->size();

	if (iformat == GL_RGBA4 || iformat == GL_RGBA || iformat == 4)
	{
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
	}
	else
	if ( iformat == GL_RGB || iformat == GL_RGB5_A1)
	{
		format = GL_RGB;
		type = GL_UNSIGNED_BYTE;
	}
	else
	if (iformat == GL_LUMINANCE_ALPHA)
	{
		format = GL_LUMINANCE_ALPHA;
		type = GL_UNSIGNED_BYTE;
	}
	else
	if (iformat == GL_LUMINANCE)
	{
		format = GL_LUMINANCE;
		type = GL_UNSIGNED_BYTE;
	}
	else
	if (iformat == GL_DEPTH_COMPONENT16 || iformat == GL_DEPTH_COMPONENT)
	{
		format = GL_DEPTH_COMPONENT;
		type = GL_UNSIGNED_INT;
		attachment = GL_DEPTH_ATTACHMENT;
		filt = GL_NEAREST;
	}
	else
	{
		return;
	}
	byte* buff = new byte[_width * _height * 4];

	glGenTextures(1, tex_id);
	glBindFramebuffer(GL_FRAMEBUFFER, _frame_id);
	glBindTexture(GL_TEXTURE_2D, tex_id[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, iformat, _width, _height, 0, format, type, buff);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filt);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filt);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex_id[0], 0);
	delete[] buff;

	glBindFramebuffer(GL_FRAMEBUFFER, _frame_id);
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		CLog::getInstance()->addError("CFrameBuffer::attachTexture() \t| FrameBuffer create error");
	}

	_tex_id->push_back(tex_id[0]);
	_buffers[_tex_id->size() - 1] = attachment;

	delete[] tex_id;
}

void CFrameBuffer::deattachTextures()
{
	if (_tex_id != NULL)
	{
		for each (GLuint tex_id in (*_tex_id))
		{
			GLuint* texId = new GLuint[1];
			texId[0] = tex_id;
			glDeleteTextures(1, texId);
			delete[] texId;
		}
		_tex_id->clear();
	}
}
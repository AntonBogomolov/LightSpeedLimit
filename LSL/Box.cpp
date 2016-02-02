#include "Box.h"

CBox::CBox()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

CBox::CBox(const float x, const float y, const float w, const float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

CBox::~CBox()
{
}

void CBox::setDataFromBox(const CBox box)
{
	this->x = box.getX();
	this->y = box.getY();
	this->w = box.getW();
	this->h = box.getH();
}

void CBox::setData(const float x, const float y, const float w, const float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

CBox* CBox::getCopy()const
{
	return new CBox(x, y, w, h);
}

void CBox::update()
{

}

const float CBox::getX()const
{
	return x;
}

void CBox::setX(const float x)
{
	this->x = x;
}

const float CBox::getY()const
{
	return y;
}

void CBox::setY(const float y)
{
	this->y = y;
}

const float CBox::getW()const
{
	return w;
}

void CBox::setW(const float w)
{
	this->w = w;
}

const float CBox::getH()const
{
	return h;
}

void CBox::setH(const float h)
{
	this->h = h;
}
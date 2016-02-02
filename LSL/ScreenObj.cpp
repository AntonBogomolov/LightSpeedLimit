#include "ScreenObj.h"
#include "Camera.h"
#include "../../tools/includes/glm/glm.hpp"

CScreenObj::CScreenObj()
{
	this->coords = new glm::vec2[4]();

	this->rotAngle = 0.0f;
	this->width = 0;
	this->height = 0;
	updatePoints();
}

CScreenObj::CScreenObj(const glm::vec2* coords)
{	
	this->centerCoords.x = coords->x;
	this->centerCoords.y = coords->y;
	this->coords = new glm::vec2[4]();

	this->rotAngle = 0.0f;
	this->width = 0;
	this->height = 0;
	updatePoints();
}

CScreenObj::CScreenObj(const int x, const int y, const int w, const int h)
{	
	this->centerCoords.x = (float)x;
	this->centerCoords.y = (float)y;
	this->coords = new glm::vec2[4]();

	this->width = w;
	this->height = h;
	this->rotAngle = 0.0f;	
	updatePoints();
}

CScreenObj::~CScreenObj()
{
	if (coords)	delete[] coords;
}

const glm::vec2* CScreenObj::getCenterAbsoluteCoords()const
{
	return &centerCoords;
}

const glm::vec2* CScreenObj::getCenterScreenCoords()
{
	const glm::vec2  camCoord = CCamera::getInstance()->getCamCoords();
	centerScreenCoords.x = centerCoords.x - camCoord.x;
	centerScreenCoords.y = centerCoords.y - camCoord.y;
	return &centerScreenCoords;
}

void CScreenObj::setCenterAbsoluteCoords(const glm::vec2* newCoords)
{
	if (!newCoords) return;
	centerCoords.x = newCoords->x;
	centerCoords.y = newCoords->y;
	updatePoints();
}

void CScreenObj::setCenterScreenCoords(const glm::vec2* newCoords)
{
	if (!newCoords) return;
	const glm::vec2 camCoord = CCamera::getInstance()->getCamCoords();
	centerCoords.x = newCoords->x + camCoord.x;
	centerCoords.y = newCoords->y + camCoord.y;
	centerScreenCoords.x = newCoords->x;
	centerScreenCoords.y = newCoords->y;
	updatePoints();
}

void CScreenObj::setCenterAbsoluteCoords(const int x, const int y)
{
	centerCoords.x = (float)x;
	centerCoords.y = (float)y;
	updatePoints();
}

void CScreenObj::setCenterScreenCoords(const int x, const int y)
{
	const glm::vec2  camCoord = CCamera::getInstance()->getCamCoords();
	centerCoords.x = x + camCoord.x;
	centerCoords.y = y + camCoord.y;
	centerScreenCoords.x = (float)x;
	centerScreenCoords.y = (float)y;
	updatePoints();
}

const int CScreenObj::getWidth() const
{
	return width;
}

void CScreenObj::setWidth(int spriteW)
{
	this->width = spriteW;
	updatePoints();
}

const int CScreenObj::getHeight()const
{
	return height;
}

void CScreenObj::setHeight(int spriteH)
{
	this->height = spriteH;	
	updatePoints();
}

const float CScreenObj::getRotAngle()const
{
	return rotAngle;
}

void CScreenObj::setRotAngle(float angle)
{
	rotAngle = angle;
	updatePoints();
}

const glm::vec2* CScreenObj::getPointCoords(int pointNum)const
{
	if (pointNum >= 4 && pointNum < 0) return 0;
	return &coords[pointNum];
}

const int	CScreenObj::getPointCoordX(const int pointNum)const
{
	if (pointNum >= 4 && pointNum < 0) return 0;
	return (int)coords[pointNum].x;
}

const int CScreenObj::getPointCoordY(const int pointNum)const
{
	if (pointNum >= 4 && pointNum < 0) return 0;
	return (int)coords[pointNum].y;
}

const glm::vec2* CScreenObj::getPointsCoords()const
{
	return coords;
}

void  CScreenObj::updatePoints()
{
	float sinAlpha = (float)glm::sin(glm::radians(rotAngle));
	float cosAlpha = (float)glm::cos(glm::radians(rotAngle));

	coords[0].x = -width / 2.0f;
	coords[0].y = -height / 2.0f;
	coords[1].x = width / 2.0f;
	coords[1].y = -height / 2.0f;
	coords[2].x = width / 2.0f;
	coords[2].y = height / 2.0f;
	coords[3].x = -width / 2.0f;
	coords[3].y = height / 2.0f;
	
	int x = (int)coords[0].x;
	int y = (int)coords[0].y;
	int cx = (int)centerCoords.x;
	int cy = (int)centerCoords.y;
	int w = width;
	int h = height;
	coords[0].x = (cx + (x) * cosAlpha - (y) * sinAlpha);
	coords[0].y = (cy + (y)* cosAlpha + (x)* sinAlpha);
	coords[1].x = (cx + (x + w) * cosAlpha - (y)* sinAlpha);
	coords[1].y = (cy + (y)* cosAlpha + (x + w) * sinAlpha);
	coords[2].x = (cx + (x + w) * cosAlpha - (y + h) * sinAlpha);
	coords[2].y = (cy + (y + h) * cosAlpha + (x + w) * sinAlpha);
	coords[3].x = (cx + (x)* cosAlpha - (y + h) * sinAlpha);
	coords[3].y = (cy + (y + h) * cosAlpha + (x)* sinAlpha);
}
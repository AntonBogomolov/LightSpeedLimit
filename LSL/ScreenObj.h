#pragma once

#include "../../tools/includes/glm/glm.hpp"

class CScreenObj
{
private:
	glm::vec2* coords;

	void updatePoints();
	glm::vec2 centerCoords;
	glm::vec2 centerScreenCoords;

	int width;
	int height;
	float rotAngle;
public:
	const glm::vec2* getCenterAbsoluteCoords()const;
	const glm::vec2* getCenterScreenCoords();

	virtual void setCenterAbsoluteCoords(const glm::vec2* newCoords);
	virtual void setCenterScreenCoords(const glm::vec2* newCoords);
	virtual void setCenterAbsoluteCoords(const int x, const int y);
	virtual void setCenterScreenCoords(const int x, const int y);

	const glm::vec2* getPointsCoords()const;
	const glm::vec2* getPointCoords(const int pointNum)const;
	const int getPointCoordX(const int pointNum)const;
	const int getPointCoordY(const int pointNum)const;

	const int getWidth() const;
	virtual void setWidth(const int w);
	const int getHeight() const;
	virtual void setHeight(const int h);

	const float getRotAngle() const;
	virtual void setRotAngle(const float angle);

	CScreenObj();
	CScreenObj(const int x, const int y, const int w, const int h);
	CScreenObj(const glm::vec2* coords);
	virtual ~CScreenObj();
};


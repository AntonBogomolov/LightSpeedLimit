#pragma once
class CBox
{
protected:
	float x;
	float y;
	float w;
	float h;
public:
	void setDataFromBox(const CBox box);
	void setData(const float x, const float y, const float w, const float h);
	CBox* getCopy()const;

	void update();

	const float getX()const;
	const float getY()const;
	const float getW()const;
	const float getH()const;

	void setX(const float x);
	void setY(const float y);
	void setW(const float w);
	void setH(const float h);

	CBox();
	CBox(const float x, const float y, const float w, const float h);
	~CBox();
};


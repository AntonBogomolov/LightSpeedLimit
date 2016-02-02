#include "GameObj.h"

CGameObj::CGameObj(const int type) : CScreenObj()
{
	this->type = type;
}

CGameObj::CGameObj(const int type, const glm::vec2* coords) : CScreenObj(coords)
{
	this->type = type;	
}

CGameObj::CGameObj(const int type, const int x, const int y, const int w, const int h) : CScreenObj(x, y, w, h)
{
	this->type = type;	
}

CGameObj::~CGameObj()
{
	
}

const int CGameObj::getType()const
{
	return type;
}

void CGameObj::setType(const int type)
{
	this->type = type;
}
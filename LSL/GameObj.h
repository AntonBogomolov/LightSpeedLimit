#pragma once

#include "ScreenObj.h"

class CGameObj : public CScreenObj
{
private:
	int type;
public:
	static const int TYPE_SPRITE = 0;
	static const int TYPE_SPRITE_SET = 1;
	static const int TYPE_SPRITE_ANIMATED = 2;
	static const int TYPE_SPRITE_COLLECTION = 3;
	static const int TYPE_DRAWABLE = 6;
	static const int TYPE_GAME_OBJ = 7;
	static const int TYPE_GEOM_PRIMITIVE = 8;

	const int getType() const;
	void setType(const int type);

	virtual void update(const float dt) = 0;

	CGameObj(const int type);
	CGameObj(const int type, const glm::vec2* coords);
	CGameObj(const int type, const int x, const int y, const int w = 0, const int h = 0);
	virtual ~CGameObj();
};


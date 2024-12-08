#pragma once
#include "Object.h"
#include "Object_Effect.h"

class Object_Player : public Object
{
private:

	Object_Effect* m_effect;

	stBase m_player;

	enPOSITION m_pos;
	enANIMATION_STATE m_aniState;
	enCHARACTER_TYPE m_charType;
	enCHARACTER_STATE m_charState;

public:

	HRESULT init(int startPosX, int startPosY);
	void release();
	void update();
	void render(HDC hdc);

	void move();
	void skill();
	void collision();

	void change();
	void inWater();
	void frame();

	POINT getPlayerPos() { return m_player.rcPos; }
	RECT getPlayerRc() { return m_player.rc; }
	image* getPlayerImg() { return m_player.img; }

	enPOSITION getPos() { return m_pos; }
	enCHARACTER_STATE getCharState() { return m_charState; }
	enCHARACTER_TYPE getCharType() { return m_charType; }
	void setCharType(int charType) { m_charType = (enCHARACTER_TYPE)charType; }

	RECT getEffectRc() { return m_effect->getEffectRc(); }

public:
	Object_Player() {};
	~Object_Player() {};
};


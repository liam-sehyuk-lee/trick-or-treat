#pragma once
#include "Object.h"

#define df_MAXEFFECT 4
#define df_RADIUS 40

struct stSku { RECT rc[4]; int count, index; };

class Object_Effect : public Object
{
private:

	stBase m_effect[df_MAXEFFECT];
	POINT m_effectPos;
	RECT m_effectRc;
	
	stSku m_skullSkill;

	enEFFECT m_curEffect;

	POINT m_playerPos;
	enCHARACTER_TYPE m_charType;

public:

	HRESULT init();
	void release();
	void update(POINT playerPos, enPOSITION pos, enCHARACTER_TYPE charType);
	void render(HDC hdc);

	void frame();
	void setFrame();

	RECT getEffectRc() { return m_effectRc; }

public:
	Object_Effect() {};
	~Object_Effect() {};
};


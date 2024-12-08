#pragma once
#include "Object.h"

#define df_MAXNPC 4

class Object_NPC : public Object
{
private:

	image* m_npc[df_MAXNPC];

public:
	
	HRESULT init();
	void release();
	void update();
	void render(HDC hdc, enINGAME_NPC npc, int x, int y);

	int getFrameX(enINGAME_NPC npc) { return m_npc[npc]->getFrameX(); }
	void setFrameX(enINGAME_NPC npc) { return m_npc[npc]->setFrameX(1); }

	void resetFrameX(int npc) { return m_npc[npc]->setFrameX(0); }

public:
	Object_NPC() {};
	~Object_NPC() {};
};


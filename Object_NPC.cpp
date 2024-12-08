#include "stdafx.h"
#include "Object_NPC.h"

HRESULT Object_NPC::init()
{
	m_npc[IN_NPC0] = IMAGEMANAGER->addFrameImage("npc0", "images/object/npc0.bmp", 96, 43, 2, 1, true);
	m_npc[IN_NPC1] = IMAGEMANAGER->addFrameImage("npc1", "images/object/npc1.bmp", 98, 71, 2, 1, true);
	m_npc[IN_NPC2] = IMAGEMANAGER->addFrameImage("npc2", "images/object/npc2.bmp", 192, 87, 2, 1, true);
	m_npc[IN_NPC3] = IMAGEMANAGER->addFrameImage("npc3", "images/object/npc3.bmp", 156, 71, 2, 1, true);

	return S_OK;
}

void Object_NPC::release()
{
}

void Object_NPC::update()
{
}

void Object_NPC::render(HDC hdc, enINGAME_NPC npc, int x, int y)
{
	switch (npc)
	{
	case IN_NPC0:	m_npc[npc]->frameRender(hdc, x, y + 2.5);				break;
	case IN_NPC1:	m_npc[npc]->frameRender(hdc, x - 0.5, y + 6.25);		break;
	case IN_NPC2:	m_npc[npc]->frameRender(hdc, x, y + 2.25);				break;
	case IN_NPC3:	m_npc[npc]->frameRender(hdc, x + 4.5, y + 6.25);		break;
	}
}
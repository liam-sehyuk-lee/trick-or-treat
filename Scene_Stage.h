#pragma once
#include "Scene.h"
#include "Object_Player.h"
#include "Object_Item.h"
#include "Object_NPC.h"
#include "Object_House.h"
#include "Object_Lock.h"
#include "Object_Ghost.h"

#include "UI.h"

class Scene_Stage : public Scene
{
private:

	//백버퍼
	image* m_backBuffer;
	image* m_mapBuffer;

	//아이콘 위에 서있는지 유무
	bool m_bIcon[6];

	//스테이지 클리어 유무
	bool m_bStageClear;

	bool m_bWater;
	int	m_iWaterCount;

	int m_iTotalCoin, m_iTotalCandy, m_iTotalSuperCoin;

	Object_Player* m_player;
	Object_Item* m_item;
	Object_NPC* m_npc;
	Object_House* m_house;
	Object_Lock* m_lock;
	Object_Ghost* m_ghost;

	UI* m_ui;

public:

	HRESULT init();
	void release();
	void update();

	void render();

	void houseRender(HDC hdc);
	void lockRender(HDC hdc);
	void objectRender(HDC hdc);

	void collision();
	void iconAlter(int tileNum);

	void stageReset();
	void stageClear();

	void debugRect();
	void debugRender();

public:
	Scene_Stage() {};
	~Scene_Stage() {};

};


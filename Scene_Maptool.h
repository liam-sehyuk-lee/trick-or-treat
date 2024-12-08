#pragma once
#include "Scene.h"

enum enSTAGE;
enum enTILEMAP;

#define df_MAXSTAGE 4
#define df_MAXHOUSE 6
#define df_MAXLOCK 3

//샘플 타일 갯수
#define df_TERRAIN_TILEX 16
#define df_TERRAIN_TILEY 14

struct stButton { RECT rc; bool click; };						//버튼 구조체

struct stSampleTile												//샘플 타일 구조체
{
	RECT rc;
	int terrainFrameX, terrainFrameY, objFrameX, objFrameY;
};

struct stTilePos												//현재 타일 구조체
{
	enTILEMAP map; enTERRAIN terrain; enOBJECT obj; enHOUSE house; enFLYABLE fly; enLOCK lock;
	int x, y, frameX1, frameY1, frameX2, frameY2;
};

class Scene_Maptool : public Scene
{
private:

	bool m_bOverlap;
	bool m_houseOn[df_MAXHOUSE];
	bool m_lockOn[df_MAXLOCK];

	//타일
	stTilePos m_currentTile;

	//화면 오른쪽 이미지 타일 갯수
	stSampleTile m_sampleTiles[df_TERRAIN_TILEX* df_TERRAIN_TILEY];

	//세이브 로드용
	stButton m_save, m_load, m_arrowL, m_arrowR, m_delete, m_stage[df_MAXSTAGE];
	DWORD write;

public:
	Scene_Maptool() {};
	~Scene_Maptool() {};

	virtual HRESULT init();
	virtual void release();
	virtual void update();
	virtual void render();

	//맵툴을 셋팅하자
	void sampleMapSetup();
	void mainMapSetup();

	void terrainSetMap();
	void objectSetMap();

	void button();
	void save(int stageNum);
	void load(int stageNum);

	enTERRAIN terrainSelect(int frameX, int frameY);
	enOBJECT objectSelect(int frameX, int frameY);

	//선택 타일
	void curTileReset();
	void curTileRender();

	//디버그
	void debugRect();
	void debugRender();
};
#include "stdafx.h"
#include "Scene_Maptool.h"

HRESULT Scene_Maptool::init()
{
	SOUNDMANAGER->play("맵툴음악", 1.0f);

	IMAGEMANAGER->addImage("맵툴배경", "images/tilemap/bg.bmp", WINSIZEX, WINSIZEY, true);
	IMAGEMANAGER->addImage("맵툴프레임", "images/tilemap/frame.bmp", WINSIZEX, WINSIZEY, true);

	IMAGEMANAGER->addFrameImage("타일0", "images/tilemap/tile0.bmp", 768 / 2, 672 / 2, df_TERRAIN_TILEX, df_TERRAIN_TILEY, true);
	IMAGEMANAGER->addFrameImage("타일1", "images/tilemap/tile1.bmp", 240 / 2, 192 / 2, df_OBJ_TILEX, df_OBJ_TILEY, true);

	IMAGEMANAGER->addFrameImage("왼쪽화살표", "images/button/arrowL.bmp", 92, 64, 2, 1, true);
	IMAGEMANAGER->addFrameImage("오른쪽화살표", "images/button/arrowR.bmp", 92, 64, 2, 1, true);

	IMAGEMANAGER->addFrameImage("세이브", "images/button/save.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("로드", "images/button/load.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("지우개", "images/button/delete.bmp", 120, 60, 2, 1, true);

	IMAGEMANAGER->addFrameImage("스테이지1", "images/button/stage1.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("스테이지2", "images/button/stage2.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("스테이지3", "images/button/stage3.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("스테이지4", "images/button/stage4.bmp", 120, 97, 1, 2, true);

	m_arrowL.rc = RectMakeCenter(1020, 450, 92, 64);
	m_arrowR.rc = RectMakeCenter(1280, 450, 92, 64);

	m_save.rc = RectMakeCenter(1000, 700, 120, 97);
	m_load.rc = RectMakeCenter(1250, 700, 120, 97);
	m_delete.rc = RectMakeCenter(900, 670, 120, 60);

	m_stage[0].rc = RectMakeCenter(900, 600, 120, 97);
	m_stage[1].rc = RectMakeCenter(1050, 600, 120, 97);
	m_stage[2].rc = RectMakeCenter(1200, 600, 120, 97);
	m_stage[3].rc = RectMakeCenter(1350, 600, 120, 97);

	m_currentTile.x = 1;
	m_currentTile.y = 5;

	m_bDebug = false;
	m_bOverlap = false;

	curTileReset();
	m_currentTile.map = MAP_TERRAIN;
	sampleMapSetup();
	mainMapSetup();

	return S_OK;
}

void Scene_Maptool::release()
{
}

void Scene_Maptool::update()
{
	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		switch (m_currentTile.map)
		{
		case MAP_TERRAIN:
			this->terrainSetMap();
			break;

		case MAP_OBJECT:
			this->objectSetMap();
			break;
		}
	}

	button();
	debugRect();

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN)) SCENEMANAGER->changeScene("타이틀");
	if (KEYMANAGER->isOnceKeyDown(VK_TAB)) m_bDebug = !m_bDebug;
}

void Scene_Maptool::render()
{
	//흰색 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	//====================================================

	IMAGEMANAGER->render("맵툴배경", getMemDC());

	switch (m_currentTile.map)
	{
	case MAP_TERRAIN:
		IMAGEMANAGER->render("타일0", getMemDC(), WINSIZEX - IMAGEMANAGER->findImage("타일0")->getWidth() - df_TERRAIN_POSX, df_TERRAIN_POSY);
		break;

	case MAP_OBJECT:
		IMAGEMANAGER->render("타일1", getMemDC(), WINSIZEX - IMAGEMANAGER->findImage("타일1")->getWidth() - df_OBJ_POSX, df_OBJ_POSY);
		break;
	}

	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		IMAGEMANAGER->frameRender("타일0", getMemDC(), m_tiles[i].rc.left, m_tiles[i].rc.top, m_tiles[i].terrainFrameX, m_tiles[i].terrainFrameY);
		IMAGEMANAGER->frameRender("타일1", getMemDC(), m_tiles[i].rc.left, m_tiles[i].rc.top, m_tiles[i].objFrameX, m_tiles[i].objFrameY);
	}

	//버튼
	IMAGEMANAGER->frameRender("왼쪽화살표", getMemDC(), m_arrowL.rc.left, m_arrowL.rc.top, m_arrowL.click, 0);
	IMAGEMANAGER->frameRender("오른쪽화살표", getMemDC(), m_arrowR.rc.left, m_arrowR.rc.top, m_arrowR.click, 0);

	IMAGEMANAGER->frameRender("세이브", getMemDC(), m_save.rc.left, m_save.rc.top, 0, m_save.click);
	IMAGEMANAGER->frameRender("로드", getMemDC(), m_load.rc.left, m_load.rc.top, 0, m_load.click);
	IMAGEMANAGER->frameRender("지우개", getMemDC(), m_delete.rc.left, m_delete.rc.top, m_delete.click, 0);

	IMAGEMANAGER->frameRender("스테이지1", getMemDC(), m_stage[0].rc.left, m_stage[0].rc.top, 0, m_stage[0].click);
	IMAGEMANAGER->frameRender("스테이지2", getMemDC(), m_stage[1].rc.left, m_stage[1].rc.top, 0, m_stage[1].click);
	IMAGEMANAGER->frameRender("스테이지3", getMemDC(), m_stage[2].rc.left, m_stage[2].rc.top, 0, m_stage[2].click);
	IMAGEMANAGER->frameRender("스테이지4", getMemDC(), m_stage[3].rc.left, m_stage[3].rc.top, 0, m_stage[3].click);

	//UI
	for (int i = 0; i < df_OBJ_TILEY; i++)
	{
		IMAGEMANAGER->frameRender("타일1", getMemDC(), WINSIZEX - 150, df_TERRAIN_POSY + df_TILE_SIZE * i, 0, i);
		IMAGEMANAGER->frameRender("타일1", getMemDC(), WINSIZEX - 150, df_TERRAIN_POSY + df_TILE_SIZE * 4, 1, 0);
		IMAGEMANAGER->frameRender("타일1", getMemDC(), WINSIZEX - 150, 146 + df_TILE_SIZE * i, 1, i);
		IMAGEMANAGER->frameRender("타일1", getMemDC(), WINSIZEX - 150, 242 + df_TILE_SIZE * i, 2, i);

		for (int j = 0; j < 4; j++)
		{
			IMAGEMANAGER->frameRender("타일1", getMemDC(),
				WINSIZEX - 162 + df_TILE_SIZE * i,
				338 + df_TILE_SIZE * j, 3 + i, j);
		}
	}

	//숫자 계산
	char str[df_MAXOBJECT][128];

	sprintf_s(str[0], "※ 집과 npc는 각각 3개의 코인을 소지하고 있습니다");
	TextOut(getMemDC(), 950, 20, str[0], strlen(str[0]));

	for (int i = OBJ_SKULL; i < OBJ_HOUSE; i++)
	{
		if (i <= OBJ_FISHMAN) sprintf_s(str[i], ": %d / 1", m_object[i].on);
		sprintf_s(str[OBJ_COIN], ": %.f / ∞", m_object[OBJ_COIN].num);
		sprintf_s(str[OBJ_CANDY], ": %.f / ∞", m_object[OBJ_CANDY].num);
		sprintf_s(str[OBJ_SUPER_COIN], ": %d / 1", m_object[OBJ_SUPER_COIN].on);
		sprintf_s(str[OBJ_GHOST], ": %.f / ∞", m_object[OBJ_GHOST].num);
		if (i >= OBJ_NPC0) sprintf_s(str[i], ": %d / 1", m_object[i].on);

		if (i <= OBJ_NPC0) TextOut(getMemDC(), WINSIZEX - 100, 30 + df_TILE_SIZE * i, str[i], strlen(str[i]));
		else TextOut(getMemDC(), WINSIZEX - 100, 210 + 48 * (i - 9), str[i], strlen(str[i]));
	}

	sprintf_s(str[OBJ_HOUSE], "HOUSE : %.f / 5", m_object[OBJ_HOUSE].num);
	TextOut(getMemDC(), WINSIZEX - 152, 450, str[OBJ_HOUSE], strlen(str[OBJ_HOUSE]));

	if (m_bDebug) debugRender();
	curTileRender();

	IMAGEMANAGER->render("맵툴프레임", getMemDC());

	//=====================================================
	//백버퍼의 내용을 HDC에 그린다.(지우지마!!)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}

void Scene_Maptool::sampleMapSetup()
{
	switch (m_currentTile.map)
	{
	case MAP_TERRAIN:
		for (int i = 0; i < df_TERRAIN_TILEY; i++)
		{
			for (int j = 0; j < df_TERRAIN_TILEX; j++)
			{
				m_sampleTiles[i * df_TERRAIN_TILEX + j].terrainFrameX = j;
				m_sampleTiles[i * df_TERRAIN_TILEX + j].terrainFrameY = i;

				//좌표값을 설정하자
				SetRect(&m_sampleTiles[i* df_TERRAIN_TILEX + j].rc,
					(WINSIZEX - IMAGEMANAGER->findImage("타일0")->getWidth()) + j * df_TILE_SIZE - df_TERRAIN_POSX,
					i* df_TILE_SIZE + df_TERRAIN_POSY, (WINSIZEX - IMAGEMANAGER->findImage("타일0")->getWidth()) + j * df_TILE_SIZE + df_TILE_SIZE - df_TERRAIN_POSX,
					i* df_TILE_SIZE + df_TILE_SIZE + df_TERRAIN_POSY);
			}
		}
		break;

	case MAP_OBJECT:
		for (int i = 0; i < df_OBJ_TILEY; i++)
		{
			for (int j = 0; j < df_OBJ_TILEX; j++)
			{
				m_sampleTiles[i * df_OBJ_TILEX + j].objFrameX = j;
				m_sampleTiles[i * df_OBJ_TILEX + j].objFrameY = i;

				//좌표값을 설정하자
				SetRect(&m_sampleTiles[i* df_OBJ_TILEX + j].rc,
					(WINSIZEX - IMAGEMANAGER->findImage("타일1")->getWidth()) + j * df_TILE_SIZE - df_OBJ_POSX,
					i* df_TILE_SIZE + df_OBJ_POSY, (WINSIZEX - IMAGEMANAGER->findImage("타일1")->getWidth()) + j * df_TILE_SIZE + df_TILE_SIZE - df_OBJ_POSX,
					i* df_TILE_SIZE + df_TILE_SIZE + df_OBJ_POSY);
			}
		}
		break;
	}
}

void Scene_Maptool::mainMapSetup()
{
	for (int i = 0; i < df_TILEY; i++)
	{
		for (int j = 0; j < df_TILEX; j++)
		{
			SetRect(&m_tiles[i* df_TILEX + j].rc,
				j* df_TILE_SIZE,
				i* df_TILE_SIZE,
				j* df_TILE_SIZE + df_TILE_SIZE,
				i* df_TILE_SIZE + df_TILE_SIZE);
		}
	}

	//처음에 깔려있는 타일
	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		m_tiles[i].terrainFrameX = 1;
		m_tiles[i].terrainFrameY = 5;
		m_tiles[i].terrain = terrainSelect(m_tiles[i].terrainFrameX, m_tiles[i].terrainFrameY);

		m_tiles[i].objFrameX = df_EMPTY;
		m_tiles[i].objFrameY = df_EMPTY;
		m_tiles[i].obj = objectSelect(m_tiles[i].objFrameX, m_tiles[i].objFrameY);

		m_tiles[i].house = HOUSE_NONE;
		m_tiles[i].lock = LOCK_NONE;
		m_tiles[i].fly = FLY_NO;
	}

	//오브젝트 bool, int 값 초기화
	for (int i = 0; i < df_MAXOBJECT; i++)
	{
		m_object[i].on = false;
		m_object[i].num = 0;
	}

	//하우스 초기화
	for (int i = 0; i < df_MAXHOUSE; i++) m_houseOn[i] = false;

	//자물쇠 초기화
	for (int i = 0; i < df_MAXLOCK; i++) m_lockOn[i] = false;

	//버튼 초기화
	for (int i = 0; i < df_MAXSTAGE; i++)m_stage[i].click = false;
	m_arrowL.click = false;		m_arrowR.click = false;
	m_save.click = false;		m_load.click = false;
	m_delete.click = false;
}

void Scene_Maptool::terrainSetMap()
{
	//샘플타일
	for (int i = 0; i < df_TERRAIN_TILEX * df_TERRAIN_TILEY; i++)
	{
		if (PtInRect(&m_sampleTiles[i].rc, g_ptMouse))
		{
			m_currentTile.x = m_sampleTiles[i].terrainFrameX;
			m_currentTile.y = m_sampleTiles[i].terrainFrameY;

			m_currentTile.terrain = terrainSelect(m_currentTile.x, m_currentTile.y);
		}
	}

	//메인타일
	for (int i = 0; i < df_TILEX * df_TILEY; i++)
	{
		if (PtInRect(&m_tiles[i].rc, g_ptMouse))
		{
			switch (m_currentTile.terrain)
			{
			//단체 선택시
			case TR_ECT: case TR_HOUSE:

				m_bOverlap = false;

				for (int j = 0; j <= m_currentTile.frameY2 - m_currentTile.frameY1; j++)
				{
					for (int k = 0; k <= m_currentTile.frameX2 - m_currentTile.frameX1; k++)
					{
						//기존 건물과 겹치는지 검사
						if (m_tiles[i + df_TILEY * j + k].terrain == TR_ECT
							|| m_tiles[i + df_TILEY * j + k].terrain == TR_HOUSE) m_bOverlap = true;

						//기존 건물 있는지 검사
						else if (m_houseOn[m_currentTile.house]
							&& m_currentTile.terrain == TR_HOUSE) m_bOverlap = true;

						//밖으로 이미지가 나가는지 검사
						else if (i % df_TILEX + (m_currentTile.frameX2 - m_currentTile.frameX1) >= df_TILEX
							|| i + (m_currentTile.frameY2 - m_currentTile.frameY1) * df_TILEY > df_TILEX * df_TILEY) m_bOverlap = true;

						//오브젝트와 겹치는지 검사
						else if (m_tiles[i + df_TILEY * j + k].obj != OBJ_NONE) m_bOverlap = true;

						if (m_bOverlap) break;
					}
				}

				if (!m_bOverlap)
				{
					for (int j = 0; j <= m_currentTile.frameY2 - m_currentTile.frameY1; j++)
					{
						for (int k = 0; k <= m_currentTile.frameX2 - m_currentTile.frameX1; k++)
						{
							if (m_currentTile.terrain == TR_HOUSE)
							{
								m_object[OBJ_HOUSE].on = true;
								m_houseOn[m_currentTile.house] = true;

								m_tiles[i + df_TILEY * j + k].house = m_currentTile.house;
							}

							m_tiles[i + df_TILEY * j + k].terrainFrameX = m_currentTile.frameX1 + k;
							m_tiles[i + df_TILEY * j + k].terrainFrameY = m_currentTile.frameY1 + j;

							m_tiles[i + df_TILEY * j + k].terrain = m_currentTile.terrain;
						}
					}
				}

				InvalidateRect(g_hWnd, NULL, false);
				break;

			default:

				m_bOverlap = false;

				//기존 건물과 겹치는지 검사
				if (m_tiles[i].terrain == TR_ECT || m_tiles[i].terrain == TR_HOUSE) m_bOverlap = true;

				//기존 자물쇠 있는지 검사
				else if (m_lockOn[m_currentTile.lock]
					&& m_currentTile.terrain == TR_SKU) m_bOverlap = true;

				//오브젝트와 겹치는지 검사
				else if (m_tiles[i].obj != OBJ_NONE) m_bOverlap = true;

				if (!m_bOverlap)
				{
					if (m_currentTile.terrain == TR_SKU)
					{
						m_lockOn[m_currentTile.lock] = true;
						m_tiles[i].lock = m_currentTile.lock;
					}

					m_tiles[i].terrainFrameX = m_currentTile.x;
					m_tiles[i].terrainFrameY = m_currentTile.y;

					m_tiles[i].terrain = m_currentTile.terrain;
					m_tiles[i].fly = m_currentTile.fly;
				}

				InvalidateRect(g_hWnd, NULL, false);
				break;
			}
		}
	}

	//집과 코인 숫자 증가
	if (m_object[OBJ_HOUSE].on)
	{
		m_object[OBJ_HOUSE].num++;
		m_object[OBJ_COIN].num += 3;
		m_object[OBJ_HOUSE].on = false;
	}
}

void Scene_Maptool::objectSetMap()
{
	//샘플타일 
	for (int i = 0; i < df_OBJ_TILEX * df_OBJ_TILEY; i++)
	{
		if (PtInRect(&m_sampleTiles[i].rc, g_ptMouse))
		{
			m_currentTile.x = m_sampleTiles[i].objFrameX;
			m_currentTile.y = m_sampleTiles[i].objFrameY;

			m_currentTile.obj = objectSelect(m_currentTile.x, m_currentTile.y);
		}
	}

	//메인타일
	for (int i = 0; i < df_TILEX * df_TILEY; i++)
	{
		if (PtInRect(&m_tiles[i].rc, g_ptMouse))
		{
			switch (m_currentTile.obj)
			{
			//단체 선택시
			case OBJ_NPC0: case OBJ_NPC1: case OBJ_NPC2: case OBJ_NPC3:

				m_bOverlap = false;

				for (int j = 0; j <= m_currentTile.frameY2 - m_currentTile.frameY1; j++)
				{
					for (int k = 0; k <= m_currentTile.frameX2 - m_currentTile.frameX1; k++)
					{
						//terrain 타일위에 놓을수 있는지 검사
						if (m_tiles[i + df_TILEY * j + k].terrain != TR_NONE) m_bOverlap = true;

						//다른 오브젝트와 겹치는지 검사
						else if (m_tiles[i + df_TILEY * j + k].obj != OBJ_NONE) m_bOverlap = true;

						//기존 NPC가 있는지 검사
						else if (m_object[m_currentTile.obj].on
							&& OBJ_NPC0 <= m_currentTile.obj && m_currentTile.obj <= OBJ_NPC3) m_bOverlap = true;

						//밖으로 이미지가 나가는지 검사
						else if (i % df_TILEX + (m_currentTile.frameX2 - m_currentTile.frameX1) >= df_TILEX
							  || i + (m_currentTile.frameY2 - m_currentTile.frameY1) * df_TILEY > df_TILEX * df_TILEY) m_bOverlap = true;

						if (m_bOverlap) break;
					}
				}

				if (!m_bOverlap)
				{
					for (int j = 0; j <= m_currentTile.frameY2 - m_currentTile.frameY1; j++)
					{
						for (int k = 0; k <= m_currentTile.frameX2 - m_currentTile.frameX1; k++)
						{
							if (OBJ_NPC0 <= m_currentTile.obj && m_currentTile.obj <= OBJ_NPC3)
							{
								//npc bool 값 변경
								m_object[m_currentTile.obj].on = true;
								
								//코인 갯수 증가
								switch (m_currentTile.obj)
								{
								case OBJ_NPC0:		m_object[OBJ_COIN].num += 3;		break;
								case OBJ_NPC1:		m_object[OBJ_COIN].num += 1.5;		break;
								case OBJ_NPC2:		m_object[OBJ_COIN].num += 0.75;		break;
								case OBJ_NPC3:		m_object[OBJ_COIN].num += 0.75;		break;
								}
							}

							m_tiles[i + df_TILEY * j + k].objFrameX = m_currentTile.frameX1 + k;
							m_tiles[i + df_TILEY * j + k].objFrameY = m_currentTile.frameY1 + j;

							m_tiles[i + df_TILEY * j + k].obj = m_currentTile.obj;
						}
					}
				}

				InvalidateRect(g_hWnd, NULL, false);
				break;


			default:

				m_bOverlap = false;

				//terrain 타일위에 놓을수 있는지 검사
				if (m_tiles[i].terrain >= TR_ALL) m_bOverlap = true;

				//다른 오브젝트와 겹치는지 검사
				else if (m_tiles[i].obj != OBJ_NONE) m_bOverlap = true;

				//기존 캐릭터, 슈퍼코인 있는지 검사
				else if (m_object[m_currentTile.obj].on)
				{
					if (OBJ_SKULL <= m_currentTile.obj && m_currentTile.obj <= OBJ_FISHMAN) m_bOverlap = true;
					if (m_currentTile.obj == OBJ_SUPER_COIN) m_bOverlap = true;
				}



				if (!m_bOverlap)
				{
					//개수 혹은 bool 값 증가
					if (OBJ_SKULL <= m_currentTile.obj && m_currentTile.obj <= OBJ_FISHMAN) m_object[m_currentTile.obj].on = true;

					switch (m_currentTile.obj)
					{
					case OBJ_SUPER_COIN:	m_object[OBJ_SUPER_COIN].on = true;		break;
					case OBJ_COIN:			m_object[OBJ_COIN].num++;				break;
					case OBJ_CANDY:			m_object[OBJ_CANDY].num++;				break;
					case OBJ_GHOST:			m_object[OBJ_GHOST].num++;				break;
					}

					m_tiles[i].objFrameX = m_currentTile.x;
					m_tiles[i].objFrameY = m_currentTile.y;

					m_tiles[i].obj = m_currentTile.obj;
				}

				InvalidateRect(g_hWnd, NULL, false);
			}
		}
	}
}

void Scene_Maptool::button()
{
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		//샘플 맵 전환
		if (PtInRect(&m_arrowL.rc, g_ptMouse))
		{
			m_currentTile.map = MAP_TERRAIN;
			sampleMapSetup();
			curTileReset();
			m_arrowL.click = true;
		}

		if (PtInRect(&m_arrowR.rc, g_ptMouse))
		{
			m_currentTile.map = MAP_OBJECT;
			sampleMapSetup();
			curTileReset();
			m_arrowR.click = true;
		}

		
		for (int i = 0; i < df_MAXSTAGE; i++)
		{
			//스테이지 선택
			if (PtInRect(&m_stage[i].rc, g_ptMouse))
			{
				curTileReset();
				m_stage[i].click = true;

				for (int j = 0; j < df_MAXSTAGE; j++)
				{
					if (i != j) m_stage[j].click = false;
				}
			}

			//세이브
			if (PtInRect(&m_save.rc, g_ptMouse))
			{
				curTileReset();
				m_save.click = true;

				if (m_stage[i].click) save(i);
				m_stage[i].click = false;
			}

			//로드
			if (PtInRect(&m_load.rc, g_ptMouse))
			{
				curTileReset();
				m_load.click = true;

				if (m_stage[i].click) load(i);
				m_stage[i].click = false;
			}
		}

		//전체 지우기
		if (PtInRect(&m_delete.rc, g_ptMouse))
		{
			mainMapSetup();
			m_delete.click = true;
		}
	}

	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		m_arrowL.click = false;		m_arrowR.click = false;
		m_save.click = false;		m_load.click = false;
		m_delete.click = false;
	}
}

void Scene_Maptool::save(int stageNum)
{
	switch (stageNum)
	{
	case STAGE1:
		file1 = CreateFile("save/stage1-1.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage1-2.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE2:
		file1 = CreateFile("save/stage2-1.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage2-2.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE3:
		file1 = CreateFile("save/stage3-1.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage3-2.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE4:
		file1 = CreateFile("save/stage4-1.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage4-2.map", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	}
	WriteFile(file1, m_tiles, sizeof(stTile)*df_TILEX*df_TILEY, &write, NULL);
	WriteFile(file2, m_object, sizeof(stObject)*df_MAXOBJECT, &write, NULL);
	CloseHandle(file1);
	CloseHandle(file2);
}

void Scene_Maptool::load(int stageNum)
{
	switch (stageNum)
	{
	case STAGE1:
		file1 = CreateFile("save/stage1-1.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage1-2.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE2:
		file1 = CreateFile("save/stage2-1.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage2-2.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE3:
		file1 = CreateFile("save/stage3-1.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage3-2.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	case STAGE4:
		file1 = CreateFile("save/stage4-1.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		file2 = CreateFile("save/stage4-2.map", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		break;
	}
	ReadFile(file1, m_tiles, sizeof(stTile)*df_TILEX*df_TILEY, &read, NULL);
	ReadFile(file2, m_object, sizeof(stObject)*df_MAXOBJECT, &read, NULL);
	CloseHandle(file1);
	CloseHandle(file2);
}

enTERRAIN Scene_Maptool::terrainSelect(int frameX, int frameY)
{
	//여러개 동시 선택
	//TR_ETC
	if (df_TILE_FROM_TO(6, 11, 7, 13))		{ df_CUR_TILE(6, 11, 7, 13);  }
	if (df_TILE_FROM_TO(8, 11, 9, 13))		{ df_CUR_TILE(8, 11, 9, 13);  }

	//TR_HOUSE
	if		(df_TILE_FROM_TO(9, 0, 15, 6))		{ df_CUR_TILE(9, 0, 15, 6);		 m_currentTile.house = HOUSE1; }
	else if (df_TILE_FROM_TO(0, 7, 4, 10))		{ df_CUR_TILE(0, 7, 4, 10);		 m_currentTile.house = HOUSE2; }
	else if (df_TILE_FROM_TO(5, 7, 9, 10))		{ df_CUR_TILE(5, 7, 9, 10);		 m_currentTile.house = HOUSE3; }
	else if (df_TILE_FROM_TO(10, 7, 12, 13))	{ df_CUR_TILE(10, 7, 12, 13);	 m_currentTile.house = HOUSE4; }
	else if (df_TILE_FROM_TO(13, 7, 15, 12))	{ df_CUR_TILE(13, 7, 15, 12);	 m_currentTile.house = HOUSE5; }
	else																		 m_currentTile.house = HOUSE_NONE;

	//TR_FLYABLE
	if (df_TILE_FROM_TO(0, 0, 5, 3) || df_TILE(4, 6))	m_currentTile.fly = FLY_YES;
	else												m_currentTile.fly = FLY_NO;

	//TR_LCOK
	if		(df_TILE(0, 11))		m_currentTile.lock = LOCK1;
	else if (df_TILE(1, 11))		m_currentTile.lock = LOCK2;
	else							m_currentTile.lock = LOCK_NONE;

	//막힌위치
	if		df_TILE(1, 0) return TR_TOP;	//위
	else if df_TILE(0, 0) return TR_LT;		//좌,상
	else if df_TILE(5, 0) return TR_RT;		//우,상
	else if df_TILE(3, 3) return TR_LR;		//좌,우

	else if (df_TILE(0, 1) || df_TILE(0, 5) || df_TILE(1, 2) || df_TILE(1, 0)
		  || df_TILE(6, 1) || df_TILE(6, 2) || df_TILE(6, 3)) return TR_LEFT;							//좌

	else if (df_TILE(2, 5) || df_TILE(4, 2) || df_TILE(5, 1)
		  || df_TILE(8, 1) || df_TILE(8, 2) || df_TILE(8, 3)) return TR_RIGHT;							//우

	else if (df_TILE(1, 5) || df_TILE(3, 4) || df_TILE(3, 5)
		  || df_TILE(4, 4) || df_TILE(4, 5) || df_TILE(5, 4)
		  || df_TILE(5, 5) || df_TILE(5, 6) || df_TILE(6, 0)
		  || df_TILE(7, 0) || df_TILE(7, 1) || df_TILE(7, 2)
		  || df_TILE(7, 3) || df_TILE(7, 5) || df_TILE(8, 0)
		  || df_TILE(0, 13)|| df_TILE(1, 12)|| df_TILE(1, 13)
		  || df_TILE(2, 12)|| df_TILE(2, 13))				  return TR_NONE;							//없음

	else if (df_TILE_FROM_TO(6, 11, 7, 13) || df_TILE_FROM_TO(8, 11, 9, 13))	 return TR_ECT;			//기타

	else if (df_TILE_FROM_TO(9, 0, 15, 6) || df_TILE_FROM_TO(0, 7, 4, 10) || df_TILE_FROM_TO(5, 7, 9, 10)
		  || df_TILE_FROM_TO(10, 7, 12, 13) || df_TILE_FROM_TO(13, 7, 15, 12))	 return TR_HOUSE;		//플레이어 상호작용

	else if (df_TILE(0, 11) || df_TILE(1, 11))									 return TR_SKU;			//해골 상호작용
	else if (df_TILE(0, 12) || df_TILE(2, 11))									 return TR_FRK;			//프랑켄 상호작용
	else if (df_TILE_FROM_TO(3, 11, 5, 13))										 return TR_FISH_ALL;

	else return TR_ALL;	//다막힘
}

enOBJECT Scene_Maptool::objectSelect(int frameX, int frameY)
{
	//여러개 동시 선택
	//OJB_NPC
	if (df_TILE(2, 1))						{ df_CUR_TILE(2, 1, 2, 1); }
	if (df_TILE_FROM_TO(2, 2, 2, 3))		{ df_CUR_TILE(2, 2, 2, 3); }
	if (df_TILE_FROM_TO(3, 0, 4, 1))		{ df_CUR_TILE(3, 0, 4, 1); }
	if (df_TILE_FROM_TO(3, 2, 4, 3))		{ df_CUR_TILE(3, 2, 4, 3); }

	//OBJ 위치
	if		df_TILE(0, 0) return OBJ_SKULL;								//해골
	else if df_TILE(0, 1) return OBJ_DRACULA;							//드라큘라
	else if df_TILE(0, 2) return OBJ_FRANKEN;							//프랑켄
	else if df_TILE(0, 3) return OBJ_WITCH;								//마녀
	else if df_TILE(1, 0) return OBJ_FISHMAN;							//어인

	else if df_TILE(1, 1) return OBJ_COIN;								//코인
	else if df_TILE(1, 2) return OBJ_CANDY;								//캔디
	else if df_TILE(1, 3) return OBJ_SUPER_COIN;						//특별코인

	else if df_TILE(2, 0) return OBJ_GHOST;								//유령

	else if df_TILE(2, 1)					return OBJ_NPC0;			//NPC
	else if df_TILE_FROM_TO(2, 2, 2, 3)		return OBJ_NPC1;
	else if df_TILE_FROM_TO(3, 0, 4, 1)		return OBJ_NPC2;
	else if df_TILE_FROM_TO(3, 2, 4, 3)		return OBJ_NPC3;

	else return OBJ_NONE;
}

//마우스 이미지 초기화
void Scene_Maptool::curTileReset()
{
	m_currentTile.x =		df_EMPTY;
	m_currentTile.y =		df_EMPTY;
	m_currentTile.frameX1 = df_EMPTY;
	m_currentTile.frameY1 = df_EMPTY;
	m_currentTile.frameX2 = df_EMPTY;
	m_currentTile.frameY2 = df_EMPTY;
	m_currentTile.terrain = TR_ALL;
	m_currentTile.obj =		OBJ_NONE;
	m_currentTile.house =	HOUSE_NONE;
	m_currentTile.lock =	LOCK_NONE;
	m_currentTile.fly =		FLY_NO;
}

//마우스 위치에 현재 이미지 표시
void Scene_Maptool::curTileRender()
{
	switch (m_currentTile.map)
	{
	case MAP_TERRAIN:

		switch (m_currentTile.terrain)
		{
		case TR_ECT: case TR_HOUSE:

			for (int i = m_currentTile.frameX1; i <= m_currentTile.frameX2; i++)
			{
				for (int j = m_currentTile.frameY1; j <= m_currentTile.frameY2; j++)
				{
					IMAGEMANAGER->frameRender("타일0", getMemDC(),
						(g_ptMouse.x - df_TILE_SIZE / 2) + df_TILE_SIZE * (i - m_currentTile.frameX1),
						(g_ptMouse.y - df_TILE_SIZE / 2) + df_TILE_SIZE * (j - m_currentTile.frameY1), i, j);
				}
			}
			break;

		default:
			IMAGEMANAGER->frameRender("타일0", getMemDC(), g_ptMouse.x - df_TILE_SIZE / 2, g_ptMouse.y - df_TILE_SIZE / 2, m_currentTile.x, m_currentTile.y);
			break;
		}
		break;

	case MAP_OBJECT:

		switch (m_currentTile.obj)
		{
		case OBJ_NPC0: case OBJ_NPC1: case OBJ_NPC2: case OBJ_NPC3:

			for (int i = m_currentTile.frameX1; i <= m_currentTile.frameX2; i++)
			{
				for (int j = m_currentTile.frameY1; j <= m_currentTile.frameY2; j++)
				{
					IMAGEMANAGER->frameRender("타일1", getMemDC(),
						(g_ptMouse.x - df_TILE_SIZE / 2) + df_TILE_SIZE * (i - m_currentTile.frameX1),
						(g_ptMouse.y - df_TILE_SIZE / 2) + df_TILE_SIZE * (j - m_currentTile.frameY1), i, j);
				}
			}
			break;

		default:
			IMAGEMANAGER->frameRender("타일1", getMemDC(), g_ptMouse.x - df_TILE_SIZE / 2, g_ptMouse.y - df_TILE_SIZE / 2, m_currentTile.x, m_currentTile.y);
			break;
		}
		break;
	}
}

void Scene_Maptool::debugRect()
{
	for (int i = 0; i < df_TILEX * df_TILEY; i++)
	{
		switch (m_tiles[i].terrain)
		{
		case TR_TOP:		df_DEBUG_RECT(0, 0, 0, df_TILE_SIZE, 3);							break;
		case TR_LEFT:		df_DEBUG_RECT(0, 0, 0, 5, df_TILE_SIZE);							break;
		case TR_RIGHT:		df_DEBUG_RECT(0, df_TILE_SIZE, 0, -5, df_TILE_SIZE);				break;
		case TR_LT:			df_DEBUG_RECT(0, 0, 0, 5, df_TILE_SIZE);
							df_DEBUG_RECT(1, 0, 0, df_TILE_SIZE, 3);							break;
		case TR_RT:			df_DEBUG_RECT(0, df_TILE_SIZE, 0, -5, df_TILE_SIZE);
							df_DEBUG_RECT(1, 0, 0, df_TILE_SIZE, 3);							break;
		case TR_LR:			df_DEBUG_RECT(0, 0, 0, 3, df_TILE_SIZE);
							df_DEBUG_RECT(1, df_TILE_SIZE, 0, -3, df_TILE_SIZE);				break;

		case TR_ALL: case TR_ECT: case TR_HOUSE: case TR_SKU: case TR_FRK: case TR_FISH_ALL:
							df_DEBUG_RECT(0, 0, 0, df_TILE_SIZE, df_TILE_SIZE);					break;

		case TR_FISH_NONE:	df_DEBUG_RECT(0, 0, 0, 0, 0);
							df_DEBUG_RECT(1, 0, 0, 0, 0);
							df_DEBUG_RECT(3, 0, 0, df_TILE_SIZE, df_TILE_SIZE);					break;

		case TR_NONE:
							df_DEBUG_RECT(0, 0, 0, 0, 0);
							df_DEBUG_RECT(1, 0, 0, 0, 0);										break;
		}

		switch (m_tiles[i].obj)
		{
		case OBJ_NONE:		df_DEBUG_RECT(2, 0, 0, 0, 0);										break;
		default:			df_DEBUG_RECT(2, 5, 5, 15, 15);										break;
		}
	}
}

void Scene_Maptool::debugRender()
{
	//선그리기

	HPEN Pen1 = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
	HPEN oldPen1 = (HPEN)SelectObject(getMemDC(), Pen1);

	//메인화면 가로선
	for (int i = 0; i <= df_TILE_SIZEY; i += df_TILE_SIZE)
	{
		MoveToEx(getMemDC(), 0, i, NULL);
		LineTo(getMemDC(), df_TILE_SIZEX, i);
	}

	//메인화면 세로선
	for (int i = 0; i <= df_TILE_SIZEX; i += df_TILE_SIZE)
	{
		MoveToEx(getMemDC(), i, 0, NULL);
		LineTo(getMemDC(), i, df_TILE_SIZEY);
	}

	switch (m_currentTile.map)
	{
	case MAP_TERRAIN:

		//샘플화면 가로선	
		for (int i = 0; i <= df_TERRAIN_TILE_SIZEY; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), WINSIZEX - df_TERRAIN_POSX, i + df_TERRAIN_POSY, NULL);
			LineTo(getMemDC(), WINSIZEX - df_TERRAIN_POSX - df_TERRAIN_TILE_SIZEX, i + df_TERRAIN_POSY);
		}

		//샘플화면 세로선
		for (int i = 0; i <= df_TERRAIN_TILE_SIZEX; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), (WINSIZEX - df_TERRAIN_POSX) - i, df_TERRAIN_POSY, NULL);
			LineTo(getMemDC(), (WINSIZEX - df_TERRAIN_POSX) - i, df_TERRAIN_TILE_SIZEY + df_TERRAIN_POSY);
		}
		break;

	case MAP_OBJECT:

		//샘플화면 가로선	
		for (int i = 0; i <= df_OBJ_TILE_SIZEY; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), WINSIZEX - df_OBJ_POSX, i + df_OBJ_POSY, NULL);
			LineTo(getMemDC(), WINSIZEX - df_OBJ_POSX - df_OBJ_TILE_SIZEX, i + df_OBJ_POSY);
		}

		//샘플화면 세로선
		for (int i = 0; i <= df_OBJ_TILE_SIZEX; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), (WINSIZEX - df_OBJ_POSX) - i, df_OBJ_POSY, NULL);
			LineTo(getMemDC(), (WINSIZEX - df_OBJ_POSX) - i, df_OBJ_TILE_SIZEY + df_OBJ_POSY);
		}
		break;
	}

	SelectObject(getMemDC(), oldPen1);
	DeleteObject(Pen1);

	//렉트 그리기

	HPEN Pen2 = CreatePen(PS_NULL, NULL, NULL);
	HPEN oldPen2 = (HPEN)SelectObject(getMemDC(), Pen2);

	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			//상호작용
			if (m_tiles[i].terrain == TR_HOUSE || m_tiles[i].terrain == TR_SKU || m_tiles[i].terrain == TR_FRK || m_tiles[i].terrain == TR_FISH_ALL)
			{
				HBRUSH Brush1 = CreateSolidBrush(RGB(0, 200, 0));
				HBRUSH oldBrush1 = (HBRUSH)SelectObject(getMemDC(), Brush1);

				Rectangle(getMemDC(), m_tiles[i].rcDebug[j].left, m_tiles[i].rcDebug[j].top, m_tiles[i].rcDebug[j].right, m_tiles[i].rcDebug[j].bottom);

				SelectObject(getMemDC(), oldBrush1);
				DeleteObject(Brush1);
			}

			else
			{
				HBRUSH Brush2 = CreateSolidBrush(RGB(200, 0, 0));
				HBRUSH oldBrush2 = (HBRUSH)SelectObject(getMemDC(), Brush2);

				Rectangle(getMemDC(), m_tiles[i].rcDebug[j].left, m_tiles[i].rcDebug[j].top, m_tiles[i].rcDebug[j].right, m_tiles[i].rcDebug[j].bottom);

				SelectObject(getMemDC(), oldBrush2);
				DeleteObject(Brush2);
			}
		}

		//오브젝트
		if (m_tiles[i].obj != OBJ_NONE)
		{
			HBRUSH Brush3 = CreateSolidBrush(RGB(200, 200, 200));
			HBRUSH oldBrush3 = (HBRUSH)SelectObject(getMemDC(), Brush3);

			Rectangle(getMemDC(), m_tiles[i].rcDebug[2].left, m_tiles[i].rcDebug[2].top, m_tiles[i].rcDebug[2].right, m_tiles[i].rcDebug[2].bottom);

			SelectObject(getMemDC(), oldBrush3);
			DeleteObject(Brush3);
		}
	}

	SelectObject(getMemDC(), oldPen2);
	DeleteObject(Pen2);
}
#include "stdafx.h"
#include "Scene_Maptool.h"

HRESULT Scene_Maptool::init()
{
	SOUNDMANAGER->play("��������", 1.0f);

	IMAGEMANAGER->addImage("�������", "images/tilemap/bg.bmp", WINSIZEX, WINSIZEY, true);
	IMAGEMANAGER->addImage("����������", "images/tilemap/frame.bmp", WINSIZEX, WINSIZEY, true);

	IMAGEMANAGER->addFrameImage("Ÿ��0", "images/tilemap/tile0.bmp", 768 / 2, 672 / 2, df_TERRAIN_TILEX, df_TERRAIN_TILEY, true);
	IMAGEMANAGER->addFrameImage("Ÿ��1", "images/tilemap/tile1.bmp", 240 / 2, 192 / 2, df_OBJ_TILEX, df_OBJ_TILEY, true);

	IMAGEMANAGER->addFrameImage("����ȭ��ǥ", "images/button/arrowL.bmp", 92, 64, 2, 1, true);
	IMAGEMANAGER->addFrameImage("������ȭ��ǥ", "images/button/arrowR.bmp", 92, 64, 2, 1, true);

	IMAGEMANAGER->addFrameImage("���̺�", "images/button/save.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("�ε�", "images/button/load.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("���찳", "images/button/delete.bmp", 120, 60, 2, 1, true);

	IMAGEMANAGER->addFrameImage("��������1", "images/button/stage1.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("��������2", "images/button/stage2.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("��������3", "images/button/stage3.bmp", 120, 97, 1, 2, true);
	IMAGEMANAGER->addFrameImage("��������4", "images/button/stage4.bmp", 120, 97, 1, 2, true);

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

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN)) SCENEMANAGER->changeScene("Ÿ��Ʋ");
	if (KEYMANAGER->isOnceKeyDown(VK_TAB)) m_bDebug = !m_bDebug;
}

void Scene_Maptool::render()
{
	//��� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
	//====================================================

	IMAGEMANAGER->render("�������", getMemDC());

	switch (m_currentTile.map)
	{
	case MAP_TERRAIN:
		IMAGEMANAGER->render("Ÿ��0", getMemDC(), WINSIZEX - IMAGEMANAGER->findImage("Ÿ��0")->getWidth() - df_TERRAIN_POSX, df_TERRAIN_POSY);
		break;

	case MAP_OBJECT:
		IMAGEMANAGER->render("Ÿ��1", getMemDC(), WINSIZEX - IMAGEMANAGER->findImage("Ÿ��1")->getWidth() - df_OBJ_POSX, df_OBJ_POSY);
		break;
	}

	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		IMAGEMANAGER->frameRender("Ÿ��0", getMemDC(), m_tiles[i].rc.left, m_tiles[i].rc.top, m_tiles[i].terrainFrameX, m_tiles[i].terrainFrameY);
		IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(), m_tiles[i].rc.left, m_tiles[i].rc.top, m_tiles[i].objFrameX, m_tiles[i].objFrameY);
	}

	//��ư
	IMAGEMANAGER->frameRender("����ȭ��ǥ", getMemDC(), m_arrowL.rc.left, m_arrowL.rc.top, m_arrowL.click, 0);
	IMAGEMANAGER->frameRender("������ȭ��ǥ", getMemDC(), m_arrowR.rc.left, m_arrowR.rc.top, m_arrowR.click, 0);

	IMAGEMANAGER->frameRender("���̺�", getMemDC(), m_save.rc.left, m_save.rc.top, 0, m_save.click);
	IMAGEMANAGER->frameRender("�ε�", getMemDC(), m_load.rc.left, m_load.rc.top, 0, m_load.click);
	IMAGEMANAGER->frameRender("���찳", getMemDC(), m_delete.rc.left, m_delete.rc.top, m_delete.click, 0);

	IMAGEMANAGER->frameRender("��������1", getMemDC(), m_stage[0].rc.left, m_stage[0].rc.top, 0, m_stage[0].click);
	IMAGEMANAGER->frameRender("��������2", getMemDC(), m_stage[1].rc.left, m_stage[1].rc.top, 0, m_stage[1].click);
	IMAGEMANAGER->frameRender("��������3", getMemDC(), m_stage[2].rc.left, m_stage[2].rc.top, 0, m_stage[2].click);
	IMAGEMANAGER->frameRender("��������4", getMemDC(), m_stage[3].rc.left, m_stage[3].rc.top, 0, m_stage[3].click);

	//UI
	for (int i = 0; i < df_OBJ_TILEY; i++)
	{
		IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(), WINSIZEX - 150, df_TERRAIN_POSY + df_TILE_SIZE * i, 0, i);
		IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(), WINSIZEX - 150, df_TERRAIN_POSY + df_TILE_SIZE * 4, 1, 0);
		IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(), WINSIZEX - 150, 146 + df_TILE_SIZE * i, 1, i);
		IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(), WINSIZEX - 150, 242 + df_TILE_SIZE * i, 2, i);

		for (int j = 0; j < 4; j++)
		{
			IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(),
				WINSIZEX - 162 + df_TILE_SIZE * i,
				338 + df_TILE_SIZE * j, 3 + i, j);
		}
	}

	//���� ���
	char str[df_MAXOBJECT][128];

	sprintf_s(str[0], "�� ���� npc�� ���� 3���� ������ �����ϰ� �ֽ��ϴ�");
	TextOut(getMemDC(), 950, 20, str[0], strlen(str[0]));

	for (int i = OBJ_SKULL; i < OBJ_HOUSE; i++)
	{
		if (i <= OBJ_FISHMAN) sprintf_s(str[i], ": %d / 1", m_object[i].on);
		sprintf_s(str[OBJ_COIN], ": %.f / ��", m_object[OBJ_COIN].num);
		sprintf_s(str[OBJ_CANDY], ": %.f / ��", m_object[OBJ_CANDY].num);
		sprintf_s(str[OBJ_SUPER_COIN], ": %d / 1", m_object[OBJ_SUPER_COIN].on);
		sprintf_s(str[OBJ_GHOST], ": %.f / ��", m_object[OBJ_GHOST].num);
		if (i >= OBJ_NPC0) sprintf_s(str[i], ": %d / 1", m_object[i].on);

		if (i <= OBJ_NPC0) TextOut(getMemDC(), WINSIZEX - 100, 30 + df_TILE_SIZE * i, str[i], strlen(str[i]));
		else TextOut(getMemDC(), WINSIZEX - 100, 210 + 48 * (i - 9), str[i], strlen(str[i]));
	}

	sprintf_s(str[OBJ_HOUSE], "HOUSE : %.f / 5", m_object[OBJ_HOUSE].num);
	TextOut(getMemDC(), WINSIZEX - 152, 450, str[OBJ_HOUSE], strlen(str[OBJ_HOUSE]));

	if (m_bDebug) debugRender();
	curTileRender();

	IMAGEMANAGER->render("����������", getMemDC());

	//=====================================================
	//������� ������ HDC�� �׸���.(��������!!)
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

				//��ǥ���� ��������
				SetRect(&m_sampleTiles[i* df_TERRAIN_TILEX + j].rc,
					(WINSIZEX - IMAGEMANAGER->findImage("Ÿ��0")->getWidth()) + j * df_TILE_SIZE - df_TERRAIN_POSX,
					i* df_TILE_SIZE + df_TERRAIN_POSY, (WINSIZEX - IMAGEMANAGER->findImage("Ÿ��0")->getWidth()) + j * df_TILE_SIZE + df_TILE_SIZE - df_TERRAIN_POSX,
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

				//��ǥ���� ��������
				SetRect(&m_sampleTiles[i* df_OBJ_TILEX + j].rc,
					(WINSIZEX - IMAGEMANAGER->findImage("Ÿ��1")->getWidth()) + j * df_TILE_SIZE - df_OBJ_POSX,
					i* df_TILE_SIZE + df_OBJ_POSY, (WINSIZEX - IMAGEMANAGER->findImage("Ÿ��1")->getWidth()) + j * df_TILE_SIZE + df_TILE_SIZE - df_OBJ_POSX,
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

	//ó���� ����ִ� Ÿ��
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

	//������Ʈ bool, int �� �ʱ�ȭ
	for (int i = 0; i < df_MAXOBJECT; i++)
	{
		m_object[i].on = false;
		m_object[i].num = 0;
	}

	//�Ͽ콺 �ʱ�ȭ
	for (int i = 0; i < df_MAXHOUSE; i++) m_houseOn[i] = false;

	//�ڹ��� �ʱ�ȭ
	for (int i = 0; i < df_MAXLOCK; i++) m_lockOn[i] = false;

	//��ư �ʱ�ȭ
	for (int i = 0; i < df_MAXSTAGE; i++)m_stage[i].click = false;
	m_arrowL.click = false;		m_arrowR.click = false;
	m_save.click = false;		m_load.click = false;
	m_delete.click = false;
}

void Scene_Maptool::terrainSetMap()
{
	//����Ÿ��
	for (int i = 0; i < df_TERRAIN_TILEX * df_TERRAIN_TILEY; i++)
	{
		if (PtInRect(&m_sampleTiles[i].rc, g_ptMouse))
		{
			m_currentTile.x = m_sampleTiles[i].terrainFrameX;
			m_currentTile.y = m_sampleTiles[i].terrainFrameY;

			m_currentTile.terrain = terrainSelect(m_currentTile.x, m_currentTile.y);
		}
	}

	//����Ÿ��
	for (int i = 0; i < df_TILEX * df_TILEY; i++)
	{
		if (PtInRect(&m_tiles[i].rc, g_ptMouse))
		{
			switch (m_currentTile.terrain)
			{
			//��ü ���ý�
			case TR_ECT: case TR_HOUSE:

				m_bOverlap = false;

				for (int j = 0; j <= m_currentTile.frameY2 - m_currentTile.frameY1; j++)
				{
					for (int k = 0; k <= m_currentTile.frameX2 - m_currentTile.frameX1; k++)
					{
						//���� �ǹ��� ��ġ���� �˻�
						if (m_tiles[i + df_TILEY * j + k].terrain == TR_ECT
							|| m_tiles[i + df_TILEY * j + k].terrain == TR_HOUSE) m_bOverlap = true;

						//���� �ǹ� �ִ��� �˻�
						else if (m_houseOn[m_currentTile.house]
							&& m_currentTile.terrain == TR_HOUSE) m_bOverlap = true;

						//������ �̹����� �������� �˻�
						else if (i % df_TILEX + (m_currentTile.frameX2 - m_currentTile.frameX1) >= df_TILEX
							|| i + (m_currentTile.frameY2 - m_currentTile.frameY1) * df_TILEY > df_TILEX * df_TILEY) m_bOverlap = true;

						//������Ʈ�� ��ġ���� �˻�
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

				//���� �ǹ��� ��ġ���� �˻�
				if (m_tiles[i].terrain == TR_ECT || m_tiles[i].terrain == TR_HOUSE) m_bOverlap = true;

				//���� �ڹ��� �ִ��� �˻�
				else if (m_lockOn[m_currentTile.lock]
					&& m_currentTile.terrain == TR_SKU) m_bOverlap = true;

				//������Ʈ�� ��ġ���� �˻�
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

	//���� ���� ���� ����
	if (m_object[OBJ_HOUSE].on)
	{
		m_object[OBJ_HOUSE].num++;
		m_object[OBJ_COIN].num += 3;
		m_object[OBJ_HOUSE].on = false;
	}
}

void Scene_Maptool::objectSetMap()
{
	//����Ÿ�� 
	for (int i = 0; i < df_OBJ_TILEX * df_OBJ_TILEY; i++)
	{
		if (PtInRect(&m_sampleTiles[i].rc, g_ptMouse))
		{
			m_currentTile.x = m_sampleTiles[i].objFrameX;
			m_currentTile.y = m_sampleTiles[i].objFrameY;

			m_currentTile.obj = objectSelect(m_currentTile.x, m_currentTile.y);
		}
	}

	//����Ÿ��
	for (int i = 0; i < df_TILEX * df_TILEY; i++)
	{
		if (PtInRect(&m_tiles[i].rc, g_ptMouse))
		{
			switch (m_currentTile.obj)
			{
			//��ü ���ý�
			case OBJ_NPC0: case OBJ_NPC1: case OBJ_NPC2: case OBJ_NPC3:

				m_bOverlap = false;

				for (int j = 0; j <= m_currentTile.frameY2 - m_currentTile.frameY1; j++)
				{
					for (int k = 0; k <= m_currentTile.frameX2 - m_currentTile.frameX1; k++)
					{
						//terrain Ÿ������ ������ �ִ��� �˻�
						if (m_tiles[i + df_TILEY * j + k].terrain != TR_NONE) m_bOverlap = true;

						//�ٸ� ������Ʈ�� ��ġ���� �˻�
						else if (m_tiles[i + df_TILEY * j + k].obj != OBJ_NONE) m_bOverlap = true;

						//���� NPC�� �ִ��� �˻�
						else if (m_object[m_currentTile.obj].on
							&& OBJ_NPC0 <= m_currentTile.obj && m_currentTile.obj <= OBJ_NPC3) m_bOverlap = true;

						//������ �̹����� �������� �˻�
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
								//npc bool �� ����
								m_object[m_currentTile.obj].on = true;
								
								//���� ���� ����
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

				//terrain Ÿ������ ������ �ִ��� �˻�
				if (m_tiles[i].terrain >= TR_ALL) m_bOverlap = true;

				//�ٸ� ������Ʈ�� ��ġ���� �˻�
				else if (m_tiles[i].obj != OBJ_NONE) m_bOverlap = true;

				//���� ĳ����, �������� �ִ��� �˻�
				else if (m_object[m_currentTile.obj].on)
				{
					if (OBJ_SKULL <= m_currentTile.obj && m_currentTile.obj <= OBJ_FISHMAN) m_bOverlap = true;
					if (m_currentTile.obj == OBJ_SUPER_COIN) m_bOverlap = true;
				}



				if (!m_bOverlap)
				{
					//���� Ȥ�� bool �� ����
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
		//���� �� ��ȯ
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
			//�������� ����
			if (PtInRect(&m_stage[i].rc, g_ptMouse))
			{
				curTileReset();
				m_stage[i].click = true;

				for (int j = 0; j < df_MAXSTAGE; j++)
				{
					if (i != j) m_stage[j].click = false;
				}
			}

			//���̺�
			if (PtInRect(&m_save.rc, g_ptMouse))
			{
				curTileReset();
				m_save.click = true;

				if (m_stage[i].click) save(i);
				m_stage[i].click = false;
			}

			//�ε�
			if (PtInRect(&m_load.rc, g_ptMouse))
			{
				curTileReset();
				m_load.click = true;

				if (m_stage[i].click) load(i);
				m_stage[i].click = false;
			}
		}

		//��ü �����
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
	//������ ���� ����
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

	//������ġ
	if		df_TILE(1, 0) return TR_TOP;	//��
	else if df_TILE(0, 0) return TR_LT;		//��,��
	else if df_TILE(5, 0) return TR_RT;		//��,��
	else if df_TILE(3, 3) return TR_LR;		//��,��

	else if (df_TILE(0, 1) || df_TILE(0, 5) || df_TILE(1, 2) || df_TILE(1, 0)
		  || df_TILE(6, 1) || df_TILE(6, 2) || df_TILE(6, 3)) return TR_LEFT;							//��

	else if (df_TILE(2, 5) || df_TILE(4, 2) || df_TILE(5, 1)
		  || df_TILE(8, 1) || df_TILE(8, 2) || df_TILE(8, 3)) return TR_RIGHT;							//��

	else if (df_TILE(1, 5) || df_TILE(3, 4) || df_TILE(3, 5)
		  || df_TILE(4, 4) || df_TILE(4, 5) || df_TILE(5, 4)
		  || df_TILE(5, 5) || df_TILE(5, 6) || df_TILE(6, 0)
		  || df_TILE(7, 0) || df_TILE(7, 1) || df_TILE(7, 2)
		  || df_TILE(7, 3) || df_TILE(7, 5) || df_TILE(8, 0)
		  || df_TILE(0, 13)|| df_TILE(1, 12)|| df_TILE(1, 13)
		  || df_TILE(2, 12)|| df_TILE(2, 13))				  return TR_NONE;							//����

	else if (df_TILE_FROM_TO(6, 11, 7, 13) || df_TILE_FROM_TO(8, 11, 9, 13))	 return TR_ECT;			//��Ÿ

	else if (df_TILE_FROM_TO(9, 0, 15, 6) || df_TILE_FROM_TO(0, 7, 4, 10) || df_TILE_FROM_TO(5, 7, 9, 10)
		  || df_TILE_FROM_TO(10, 7, 12, 13) || df_TILE_FROM_TO(13, 7, 15, 12))	 return TR_HOUSE;		//�÷��̾� ��ȣ�ۿ�

	else if (df_TILE(0, 11) || df_TILE(1, 11))									 return TR_SKU;			//�ذ� ��ȣ�ۿ�
	else if (df_TILE(0, 12) || df_TILE(2, 11))									 return TR_FRK;			//������ ��ȣ�ۿ�
	else if (df_TILE_FROM_TO(3, 11, 5, 13))										 return TR_FISH_ALL;

	else return TR_ALL;	//�ٸ���
}

enOBJECT Scene_Maptool::objectSelect(int frameX, int frameY)
{
	//������ ���� ����
	//OJB_NPC
	if (df_TILE(2, 1))						{ df_CUR_TILE(2, 1, 2, 1); }
	if (df_TILE_FROM_TO(2, 2, 2, 3))		{ df_CUR_TILE(2, 2, 2, 3); }
	if (df_TILE_FROM_TO(3, 0, 4, 1))		{ df_CUR_TILE(3, 0, 4, 1); }
	if (df_TILE_FROM_TO(3, 2, 4, 3))		{ df_CUR_TILE(3, 2, 4, 3); }

	//OBJ ��ġ
	if		df_TILE(0, 0) return OBJ_SKULL;								//�ذ�
	else if df_TILE(0, 1) return OBJ_DRACULA;							//���ŧ��
	else if df_TILE(0, 2) return OBJ_FRANKEN;							//������
	else if df_TILE(0, 3) return OBJ_WITCH;								//����
	else if df_TILE(1, 0) return OBJ_FISHMAN;							//����

	else if df_TILE(1, 1) return OBJ_COIN;								//����
	else if df_TILE(1, 2) return OBJ_CANDY;								//ĵ��
	else if df_TILE(1, 3) return OBJ_SUPER_COIN;						//Ư������

	else if df_TILE(2, 0) return OBJ_GHOST;								//����

	else if df_TILE(2, 1)					return OBJ_NPC0;			//NPC
	else if df_TILE_FROM_TO(2, 2, 2, 3)		return OBJ_NPC1;
	else if df_TILE_FROM_TO(3, 0, 4, 1)		return OBJ_NPC2;
	else if df_TILE_FROM_TO(3, 2, 4, 3)		return OBJ_NPC3;

	else return OBJ_NONE;
}

//���콺 �̹��� �ʱ�ȭ
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

//���콺 ��ġ�� ���� �̹��� ǥ��
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
					IMAGEMANAGER->frameRender("Ÿ��0", getMemDC(),
						(g_ptMouse.x - df_TILE_SIZE / 2) + df_TILE_SIZE * (i - m_currentTile.frameX1),
						(g_ptMouse.y - df_TILE_SIZE / 2) + df_TILE_SIZE * (j - m_currentTile.frameY1), i, j);
				}
			}
			break;

		default:
			IMAGEMANAGER->frameRender("Ÿ��0", getMemDC(), g_ptMouse.x - df_TILE_SIZE / 2, g_ptMouse.y - df_TILE_SIZE / 2, m_currentTile.x, m_currentTile.y);
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
					IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(),
						(g_ptMouse.x - df_TILE_SIZE / 2) + df_TILE_SIZE * (i - m_currentTile.frameX1),
						(g_ptMouse.y - df_TILE_SIZE / 2) + df_TILE_SIZE * (j - m_currentTile.frameY1), i, j);
				}
			}
			break;

		default:
			IMAGEMANAGER->frameRender("Ÿ��1", getMemDC(), g_ptMouse.x - df_TILE_SIZE / 2, g_ptMouse.y - df_TILE_SIZE / 2, m_currentTile.x, m_currentTile.y);
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
	//���׸���

	HPEN Pen1 = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
	HPEN oldPen1 = (HPEN)SelectObject(getMemDC(), Pen1);

	//����ȭ�� ���μ�
	for (int i = 0; i <= df_TILE_SIZEY; i += df_TILE_SIZE)
	{
		MoveToEx(getMemDC(), 0, i, NULL);
		LineTo(getMemDC(), df_TILE_SIZEX, i);
	}

	//����ȭ�� ���μ�
	for (int i = 0; i <= df_TILE_SIZEX; i += df_TILE_SIZE)
	{
		MoveToEx(getMemDC(), i, 0, NULL);
		LineTo(getMemDC(), i, df_TILE_SIZEY);
	}

	switch (m_currentTile.map)
	{
	case MAP_TERRAIN:

		//����ȭ�� ���μ�	
		for (int i = 0; i <= df_TERRAIN_TILE_SIZEY; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), WINSIZEX - df_TERRAIN_POSX, i + df_TERRAIN_POSY, NULL);
			LineTo(getMemDC(), WINSIZEX - df_TERRAIN_POSX - df_TERRAIN_TILE_SIZEX, i + df_TERRAIN_POSY);
		}

		//����ȭ�� ���μ�
		for (int i = 0; i <= df_TERRAIN_TILE_SIZEX; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), (WINSIZEX - df_TERRAIN_POSX) - i, df_TERRAIN_POSY, NULL);
			LineTo(getMemDC(), (WINSIZEX - df_TERRAIN_POSX) - i, df_TERRAIN_TILE_SIZEY + df_TERRAIN_POSY);
		}
		break;

	case MAP_OBJECT:

		//����ȭ�� ���μ�	
		for (int i = 0; i <= df_OBJ_TILE_SIZEY; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), WINSIZEX - df_OBJ_POSX, i + df_OBJ_POSY, NULL);
			LineTo(getMemDC(), WINSIZEX - df_OBJ_POSX - df_OBJ_TILE_SIZEX, i + df_OBJ_POSY);
		}

		//����ȭ�� ���μ�
		for (int i = 0; i <= df_OBJ_TILE_SIZEX; i += df_TILE_SIZE)
		{
			MoveToEx(getMemDC(), (WINSIZEX - df_OBJ_POSX) - i, df_OBJ_POSY, NULL);
			LineTo(getMemDC(), (WINSIZEX - df_OBJ_POSX) - i, df_OBJ_TILE_SIZEY + df_OBJ_POSY);
		}
		break;
	}

	SelectObject(getMemDC(), oldPen1);
	DeleteObject(Pen1);

	//��Ʈ �׸���

	HPEN Pen2 = CreatePen(PS_NULL, NULL, NULL);
	HPEN oldPen2 = (HPEN)SelectObject(getMemDC(), Pen2);

	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			//��ȣ�ۿ�
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

		//������Ʈ
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
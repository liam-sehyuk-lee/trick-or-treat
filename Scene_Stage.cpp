#include "stdafx.h"
#include "Scene_Stage.h"

HRESULT Scene_Stage::init()
{
	SOUNDMANAGER->play("�ΰ�������", 1.0f);

	IMAGEMANAGER->addImage("�������", "images/tilemap/bg.bmp", WINSIZEX, WINSIZEY, true);
	IMAGEMANAGER->addFrameImage("��������_Ÿ��0", "images/tilemap/tile0.bmp", 768, 672, df_TERRAIN_TILEX, df_TERRAIN_TILEY, true);
	IMAGEMANAGER->addFrameImage("��������_Ÿ��1", "images/tilemap/tile1.bmp", 240, 192, df_OBJ_TILEX, df_OBJ_TILEY, true);

	switch (g_stage)
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
	}

	ReadFile(file1, m_tiles, sizeof(stTile)*df_TILEX*df_TILEY, &read, NULL);
	ReadFile(file2, m_object, sizeof(stObject)*df_MAXOBJECT, &read, NULL);

	CloseHandle(file1);
	CloseHandle(file2);

	//������
	m_item = new Object_Item;
	m_item->init();

	//NPC
	m_npc = new Object_NPC;
	m_npc->init();

	//house
	m_house = new Object_House;
	m_house->init();

	//lock
	m_lock = new Object_Lock;
	m_lock->init();

	//ghost
	m_ghost = new Object_Ghost;
	m_ghost->init();

	//ui
	m_ui = new UI;
	m_ui->init();

	//�÷��̾� ������ ��ġ�� ����
	m_player = new Object_Player;
	for (int i = 0; i < df_TILEX*df_TILEY; i++)
	{
		if (m_tiles[i].obj == OBJ_SKULL)
		{
			m_player->init(m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);
		}
	}

	for (int i = 0; i < 6; i++) m_bIcon[i] = false;
	m_bDebug = false;
	m_bStageClear = false;

	//�� ������
	m_iTotalCoin = m_object[OBJ_COIN].num;
	m_iTotalCandy = m_object[OBJ_CANDY].num;
	m_iTotalSuperCoin = m_object[OBJ_SUPER_COIN].on;

	m_object[OBJ_COIN].num = 0;
	m_object[OBJ_CANDY].num = 0;
	m_object[OBJ_SUPER_COIN].on = false;

	//�����
	m_mapBuffer = IMAGEMANAGER->addImage("��ü��", df_STAGE_SIZEX, df_STAGE_SIZEY);
	m_backBuffer = IMAGEMANAGER->addImage("���α׸��°�", df_STAGE_SIZEX, df_STAGE_SIZEY);
	
	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		IMAGEMANAGER->frameRender("��������_Ÿ��0", m_mapBuffer->getMemDC(), m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, m_tiles[i].terrainFrameX, m_tiles[i].terrainFrameY);
	}
	
	//������ �ٲ� npc �� ������ reset
	stageReset();
	m_bWater = false;

	return S_OK;
}

void Scene_Stage::release()
{
	//SAFE_DELETE(m_player);
	//SAFE_DELETE(m_item);
	//SAFE_DELETE(m_npc);
	//SAFE_DELETE(m_house);
	//SAFE_DELETE(m_lock);
	//SAFE_DELETE(m_ghost);
	//SAFE_DELETE(m_ui);
}

void Scene_Stage::update()
{
	if (!m_ui->getTimeOver() && !m_bStageClear)
	{
		m_player->update();
		m_ui->timer();
	}

	stageClear();

	m_item->update();
	m_npc->update();
	m_house->update();
	m_lock->update();
	m_ghost->update();
	m_ui->update();

	debugRect();

	if (!m_ui->getTimeOver() && !m_bStageClear) collision();

	if (KEYMANAGER->isOnceKeyDown(VK_TAB)) m_bDebug = !m_bDebug;
}

void Scene_Stage::render()
{
	IMAGEMANAGER->render("�������", getMemDC());

	//ī�޶� ��ġ ����
	int cameraX = m_player->getPlayerPos().x - 350;
	int cameraY = m_player->getPlayerPos().y - 350;

	if (cameraX <= 0) cameraX = 0; if (cameraX >= 830) cameraX = 830;
	if (cameraY <= 0) cameraY = 0; if (cameraY >= 830) cameraY = 830;

	//��ü�ʿ��� �Ϻθ�
	IMAGEMANAGER->render("��ü��", m_backBuffer->getMemDC(), cameraX, cameraY, cameraX, cameraY, df_CAMERA_SIZEX, df_CAMERA_SIZEY);

	houseRender(m_backBuffer->getMemDC());														//�Ͽ콺 ����
	lockRender(m_backBuffer->getMemDC());														//�ڹ��� ����
	objectRender(m_backBuffer->getMemDC());														//������ ���� + ��Ʈ
	m_player->render(m_backBuffer->getMemDC());													//�÷��̾� ����

	//���� �÷��̾�
	GdiTransparentBlt(getMemDC(), 200, 300, 300, 300,
		m_player->getPlayerImg()->getMemDC(),
		m_player->getPlayerImg()->getFrameX() * m_player->getPlayerImg()->getFrameWidth(),
		m_player->getPlayerImg()->getFrameY() * m_player->getPlayerImg()->getFrameHeight(),
		m_player->getPlayerImg()->getFrameWidth(),
		m_player->getPlayerImg()->getFrameHeight(),
		RGB(255,0,255));

	if (m_bDebug) debugRender();																//����� ����

	IMAGEMANAGER->render("���α׸��°�", getMemDC(), 700, 30, cameraX, cameraY, df_CAMERA_SIZEX, df_CAMERA_SIZEY);
	
	//������
	m_item->render(getMemDC(), IN_COIN, 50, 150);
	m_item->render(getMemDC(), IN_CANDY, 50, 198);
	m_item->render(getMemDC(), IN_SUPER_COIN, 50, 244);
	
	//���� ���
	char str[df_MAXOBJECT][128];

	sprintf_s(str[0], "�� ���� npc�� ���� 3���� ������ �����ϰ� �ֽ��ϴ�");
	TextOut(getMemDC(), 80, 120, str[0], strlen(str[0]));

	sprintf_s(str[OBJ_COIN], "%.f /%d", m_object[OBJ_COIN].num, m_iTotalCoin);
	sprintf_s(str[OBJ_CANDY], "%.f /%d", m_object[OBJ_CANDY].num, m_iTotalCandy);
	sprintf_s(str[OBJ_SUPER_COIN], "%d /%d", m_object[OBJ_SUPER_COIN].on, m_iTotalSuperCoin);

	for (int i = OBJ_COIN; i <= OBJ_SUPER_COIN; i++)
	{
		TextOut(getMemDC(), 90, 155 + 48 * (i - 6), str[i], strlen(str[i]));
	}

	m_ui->render(m_player->getCharType());	 // ui
	
	if (m_bStageClear)
	{
		m_ui->stageClear();
		
		HFONT myFont = CreateFont(30, -10, 0, 0, 1000, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, "�޸տ�ü");
		HFONT oldFont = (HFONT)SelectObject(getMemDC(), myFont);

		sprintf_s(str[1], "Ÿ�� ���ʽ� : %d", m_ui->getTimeBonus());
		TextOut(getMemDC(), 670, 300, str[1], strlen(str[1]));

		TextOut(getMemDC(), 560, 361, str[OBJ_COIN], strlen(str[OBJ_COIN]));
		TextOut(getMemDC(), 740, 361, str[OBJ_CANDY], strlen(str[OBJ_CANDY]));
		TextOut(getMemDC(), 920, 361, str[OBJ_SUPER_COIN], strlen(str[OBJ_SUPER_COIN]));

		sprintf_s(str[2], "���� : %d", g_iScore);
		TextOut(getMemDC(), 700, 420, str[2], strlen(str[2]));

		SelectObject(getMemDC(), oldFont);
		DeleteObject(myFont);
	}
}

void Scene_Stage::houseRender(HDC hdc)
{
	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		//���� �Ͽ콺 ���ֱ�
		if (m_tiles[i].house != HOUSE_NONE)
		{
			m_tiles[i].terrainFrameX = 1;
			m_tiles[i].terrainFrameY = 5;

			//������ ���� �����ϰ� ����, ��ġ�� ���� x
			if (m_tiles[i - df_TILEX].house != m_tiles[i].house && m_tiles[i - 1].house != m_tiles[i].house)
			{
				m_house->render(hdc, m_tiles[i].house, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);
			}
		}
	}
}

void Scene_Stage::lockRender(HDC hdc)
{
	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		if (m_tiles[i].terrain == TR_SKU)
		{
			//���� �ڹ��� ���ֱ�
			m_tiles[i].terrainFrameX = 1;
			m_tiles[i].terrainFrameY = 5;

			m_lock->render(hdc, m_tiles[i].lock, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);
		}
	}
}

void Scene_Stage::objectRender(HDC hdc)
{
	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		//���� ������Ʈ ���ֱ�
		m_tiles[i].objFrameX = df_EMPTY;
		m_tiles[i].objFrameY = df_EMPTY;

		//������ ���� �����ϰ� ����
		switch (m_tiles[i].obj)
		{
		case OBJ_SKULL:
			if (m_player->getCharType() == SKULL)
			{
				m_object[OBJ_SKULL].on = false;
				m_tiles[i].obj = OBJ_NONE;
				m_bIcon[OBJ_SKULL] = false;
			}
			else m_item->render(hdc, IN_ICON, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, ICON_SKULL);
			break;

		case OBJ_DRACULA:		m_item->render(hdc, IN_ICON, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, ICON_DRACULA);		break;
		case OBJ_FRANKEN:		m_item->render(hdc, IN_ICON, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, ICON_FRANKEN);		break;
		case OBJ_WITCH:			m_item->render(hdc, IN_ICON, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, ICON_WITCH);		break;
		case OBJ_FISHMAN:		m_item->render(hdc, IN_ICON, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, ICON_FISHMAN);		break;
		case OBJ_COIN:			m_item->render(hdc, IN_COIN, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);					break;
		case OBJ_CANDY:			m_item->render(hdc, IN_CANDY, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);					break;
		case OBJ_SUPER_COIN:	m_item->render(hdc, IN_SUPER_COIN, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);				break;
		case OBJ_GHOST:			m_ghost->render(hdc, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);							break;
		case OBJ_NPC0:			m_npc->render(hdc, IN_NPC0, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);						break;
		
		//��ġ�� ���� x
		case OBJ_NPC1:
			if (m_tiles[i - df_TILEX].obj != OBJ_NPC1)
			{
				m_npc->render(hdc, IN_NPC1, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);
			}																												break;

		case OBJ_NPC2:
			if (m_tiles[i - df_TILEX].obj != OBJ_NPC2 && m_tiles[i - 1].obj != OBJ_NPC2)
			{
				m_npc->render(hdc, IN_NPC2, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);
			}																												break;

		case OBJ_NPC3:
			if (m_tiles[i - df_TILEX].obj != OBJ_NPC3 && m_tiles[i - 1].obj != OBJ_NPC3)
			{
				m_npc->render(hdc, IN_NPC3, m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2);
			}																												break;
		}
	}
}

void Scene_Stage::collision()
{
	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		RECT temp;
		for (int j = 0; j < 2; j++)
		{
			//�ͷ���
			if (IntersectRect(&temp, &m_tiles[i].rcDebug[j], &m_player->getPlayerRc()))
			{
				//���� �϶� �������� �ִ� ��
				if (m_player->getCharType() == DRACULA && m_player->getCharState() == CHAR_SPELL
					&& m_tiles[i].fly == FLY_YES) break;

				//���� �϶�
				else if (m_player->getCharType() == FISHMAN && m_tiles[i].terrain == TR_FISH_ALL) break;

				else m_player->collision();

				//�Ͽ콺 ������ ��ȭ
				if (m_tiles[i].house != HOUSE_NONE
					&& !(m_player->getCharType() == DRACULA && m_player->getCharState() == CHAR_SPELL)) //����� ������ �������)
				{
					if (m_house->getFrameX(m_tiles[i].house) == 0)
					{
						SOUNDMANAGER->effectPlay("������");

						m_object[OBJ_COIN].num += 3;
						g_iScore += 30;
					}

					m_house->setFrameX(m_tiles[i].house);
					break;
				}
			}

			//��Ʈ�� �ε�������
			for (int k = 0; k < df_TILEX* df_TILEY; k++)
			{
				if (m_tiles[k].obj == OBJ_GHOST &&
					IntersectRect(&temp, &m_tiles[i].rcDebug[j], &m_tiles[k].rcDebug[2]))
				{
					m_ghost->collision();
				}
			}
		}

		//��� ��ȣ�ۿ� ��Ʈ
		if (m_player->getCharState() == CHAR_SPELL
			&& IntersectRect(&temp, &m_tiles[i].rcDebug[2], &m_player->getEffectRc()))
		{
			if (m_tiles[i].obj == OBJ_GHOST && m_player->getCharType() == WITCH)
			{
				m_ghost->setFrame();
			}
		}

		//��� ��ȣ�ۿ� �ͷ���
		if (m_player->getCharState() == CHAR_SPELL														//��ų �����
			&& IntersectRect(&temp, &m_tiles[i].rcDebug[0], &m_player->getEffectRc()))
		{
			if (m_tiles[i].terrain == TR_SKU && m_player->getCharType() == SKULL)
			{
				m_lock->setFrame(m_tiles[i].lock);
				if (m_lock->getImage(m_tiles[i].lock)->getFrameX() == m_lock->getImage(m_tiles[i].lock)->getMaxFrameX())
				{
					m_tiles[i].terrain = TR_NONE;

					IMAGEMANAGER->frameRender("��������_Ÿ��0", m_mapBuffer->getMemDC(), m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, m_tiles[i].terrainFrameX, m_tiles[i].terrainFrameY);
				}
			}

			if (m_player->getPlayerImg()->getFrameX() == m_player->getPlayerImg()->getMaxFrameX() - 1)	 //������ ������ ����������
			{
				if (m_tiles[i].terrain == TR_FRK && m_player->getCharType() == FRANKEN)
				{
					m_tiles[i].terrainFrameX = 1;
					m_tiles[i].terrainFrameY = 5;
					m_tiles[i].terrain = TR_NONE;

					IMAGEMANAGER->frameRender("��������_Ÿ��0", m_mapBuffer->getMemDC(), m_tiles[i].rc.left * 2, m_tiles[i].rc.top * 2, m_tiles[i].terrainFrameX, m_tiles[i].terrainFrameY);
				}
			}
		}

		m_iWaterCount++;

		//����
		if (m_tiles[i].terrain == TR_FISH_ALL && m_player->getCharType() == FISHMAN
			&& IntersectRect(&temp, &m_tiles[i].rcDebug[0], &m_player->getPlayerRc()))
		{
			if (!m_bWater)
			{
				if (m_iWaterCount >= 100000)
				{
					m_iWaterCount = 0;
					SOUNDMANAGER->effectPlay("ĳ����5_ȿ����");
				}

				m_bWater = true;
			}

			m_player->inWater();
		}

		if (m_tiles[i].terrain != TR_FISH_ALL && m_player->getCharType() == FISHMAN
			&& IntersectRect(&temp, &m_tiles[i].rcDebug[0], &m_player->getPlayerRc())) m_bWater = false;

		//������
		if (IntersectRect(&temp, &m_tiles[i].rcDebug[2], &m_player->getPlayerRc())
			&& !(m_player->getCharType() == DRACULA && m_player->getCharState() == CHAR_SPELL)) //����� ������ �������
		{
			switch (m_tiles[i].obj)
			{
			case OBJ_SKULL:
				if (!m_bIcon[OBJ_SKULL])
				{
					if (m_object[OBJ_SKULL].on) SOUNDMANAGER->effectPlay("������");
					m_object[OBJ_SKULL].on = false;
					iconAlter(i);
					m_player->setCharType(SKULL);
				}
				break;
			
			case OBJ_DRACULA:
				if (!m_bIcon[OBJ_DRACULA])
				{
					if (m_object[OBJ_DRACULA].on) SOUNDMANAGER->effectPlay("������");
					m_object[OBJ_DRACULA].on = false;
					iconAlter(i);
					m_player->setCharType(DRACULA);
				}
				break;
			
			case OBJ_FRANKEN:
				if (!m_bIcon[OBJ_FRANKEN])
				{
					if (m_object[OBJ_FRANKEN].on) SOUNDMANAGER->effectPlay("������");
					m_object[OBJ_FRANKEN].on = false;
					iconAlter(i);
					m_player->setCharType(FRANKEN);
				}
				break;
			
			case OBJ_WITCH:
				if (!m_bIcon[OBJ_WITCH])
				{
					if (m_object[OBJ_WITCH].on) SOUNDMANAGER->effectPlay("������");
					m_object[OBJ_WITCH].on = false;
					iconAlter(i);
					m_player->setCharType(WITCH);
				}
				break;
			
			case OBJ_FISHMAN:
				if (!m_bIcon[OBJ_FISHMAN])
				{
					if (m_object[OBJ_FISHMAN].on) SOUNDMANAGER->effectPlay("������");
					m_object[OBJ_FISHMAN].on = false;
					iconAlter(i);
					m_player->setCharType(FISHMAN);
				}
				break;
			}
		}

		if (IntersectRect(&temp, &m_tiles[i].rcDebug[2], &m_player->getPlayerRc())
			&& m_tiles[i].obj == OBJ_GHOST)	//��Ʈ
		{
			if (!m_ghost->getChanged())
			{
				m_player->collision();
				m_ghost->collision();
			}
		}

		//������ ������Ʈ
		if (IntersectRect(&temp, &m_tiles[i].rcDebug[2], &m_player->getPlayerRc())
			&& !(m_player->getCharType() == DRACULA && m_player->getCharState() == CHAR_SPELL)) //����� ������ �������
		{
			switch (m_tiles[i].obj)
			{
			case OBJ_COIN: case OBJ_CANDY:
				if (m_tiles[i].obj == OBJ_COIN) SOUNDMANAGER->effectPlay("����");
				if (m_tiles[i].obj == OBJ_CANDY) SOUNDMANAGER->effectPlay("����");

				m_object[m_tiles[i].obj].num++;
				g_iScore += 10;
				m_tiles[i].obj = OBJ_NONE;
				break;

			case OBJ_SUPER_COIN:
				SOUNDMANAGER->effectPlay("������");

				m_object[OBJ_SUPER_COIN].on = true;
				g_iScore += 50;
				m_tiles[i].obj = OBJ_NONE;
				break;

			case OBJ_NPC0:
				if (m_object[m_tiles[i].obj].on) SOUNDMANAGER->effectPlay("������");

				m_object[m_tiles[i].obj].on = false;
				if (m_npc->getFrameX(IN_NPC0) == 0)
				{
					m_object[OBJ_COIN].num += 3;
					g_iScore += 30;
				}

				m_npc->setFrameX(IN_NPC0);
				break;

			case OBJ_NPC1:
				if (m_object[m_tiles[i].obj].on) SOUNDMANAGER->effectPlay("������");

				m_object[m_tiles[i].obj].on = false;
				if (m_npc->getFrameX(IN_NPC1) == 0)
				{
					m_object[OBJ_COIN].num += 3;
					g_iScore += 30;
				}

				m_npc->setFrameX(IN_NPC1);
				break;

			case OBJ_NPC2:
				if (m_object[m_tiles[i].obj].on) SOUNDMANAGER->effectPlay("������");

				m_object[m_tiles[i].obj].on = false;
				if (m_npc->getFrameX(IN_NPC2) == 0)
				{
					m_object[OBJ_COIN].num += 3;
					g_iScore += 30;
				}

				m_npc->setFrameX(IN_NPC2);
				break;

			case OBJ_NPC3:
				if (m_object[m_tiles[i].obj].on) SOUNDMANAGER->effectPlay("������");

				m_object[m_tiles[i].obj].on = false;
				if (m_npc->getFrameX(IN_NPC3) == 0)
				{
					m_object[OBJ_COIN].num += 3;
					g_iScore += 30;
				}

				m_npc->setFrameX(IN_NPC3);
				break;
			}
		}
	}
}

void Scene_Stage::iconAlter(int tileNum)
{
	switch (m_player->getCharType())
	{
	case SKULL:		m_tiles[tileNum].obj = OBJ_SKULL;		break;
	case DRACULA:	m_tiles[tileNum].obj = OBJ_DRACULA;		break;
	case FRANKEN:	m_tiles[tileNum].obj = OBJ_FRANKEN;		break;
	case WITCH:		m_tiles[tileNum].obj = OBJ_WITCH;		break;
	case FISHMAN:	m_tiles[tileNum].obj = OBJ_FISHMAN;		break;
	}

	m_object[m_tiles[tileNum].obj].on = true;
	for (int i = 0; i < 6; i++) m_bIcon[i] = false;
	m_bIcon[m_tiles[tileNum].obj] = true;
}

void Scene_Stage::stageReset()
{
	//��ȣ�ۿ�� npc �� ������ �ʱ�ȭ
	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		for (int i = HOUSE1; i <= HOUSE5; i++) m_house->resetFrameX(i);
		for (int i = IN_NPC0; i <= IN_NPC3; i++) m_npc->resetFrameX(i);
		m_lock->resetFrame(LOCK1);
		m_lock->resetFrame(LOCK2);
		m_ghost->resetFrame();
	}
}

void Scene_Stage::stageClear()
{
	if (m_iTotalCoin == m_object[OBJ_COIN].num && !m_bStageClear)
	{
		g_iScore += m_ui->getTimeBonus();
		m_bStageClear = true;

		if (m_iTotalCandy == m_object[OBJ_CANDY].num &&
		m_iTotalSuperCoin == m_object[OBJ_SUPER_COIN].on) m_ui->setAllClear();

		else m_ui->setCoinClear();
	}

	if (m_bStageClear && KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		g_stageLock[g_stage + 1] = false;
		SOUNDMANAGER->effectPlay("����ȿ����");
		SCENEMANAGER->changeScene("����ȭ��");
	}
}

void Scene_Stage::debugRect()
{
	for (int i = 0; i < df_TILEX * df_TILEY; i++)
	{
		switch (m_tiles[i].terrain)
		{
		case TR_TOP:		df_STAGE_RECT(0, 0, 0, df_STAGE_TILE_SIZEX, 5);							break;
		case TR_LEFT:		df_STAGE_RECT(0, 0, 0, 10, df_STAGE_TILE_SIZEY);						break;
		case TR_RIGHT:		df_STAGE_RECT(0, df_STAGE_TILE_SIZEX - 10, 0, 10, df_STAGE_TILE_SIZEY);	break;
		case TR_LT:			df_STAGE_RECT(0, 0, 0, 10, df_STAGE_TILE_SIZEY);
							df_STAGE_RECT(1, 0, 0, df_STAGE_TILE_SIZEX, 5);							break;
		case TR_RT:			df_STAGE_RECT(0, df_STAGE_TILE_SIZEX - 10, 0, 10, df_STAGE_TILE_SIZEY);
							df_STAGE_RECT(1, 0, 0, df_STAGE_TILE_SIZEX, 5);							break;
		case TR_LR:			df_STAGE_RECT(0, 0, 0, 5, df_STAGE_TILE_SIZEY);
							df_STAGE_RECT(1, df_STAGE_TILE_SIZEX - 5, 0, 5, df_STAGE_TILE_SIZEY);	break;

		case TR_ALL: case TR_ECT: case TR_HOUSE: case TR_SKU: case TR_FRK: case TR_FISH_ALL:
							df_STAGE_RECT(0, 0, 0, df_STAGE_TILE_SIZEX, df_STAGE_TILE_SIZEY);		break;

		case TR_FISH_NONE:	df_STAGE_RECT(0, 0, 0, 0, 0);
							df_STAGE_RECT(1, 0, 0, 0, 0);
							df_STAGE_RECT(3, 0, 0, df_TILE_SIZE, df_TILE_SIZE);						break;

		case TR_NONE:
							df_STAGE_RECT(0, 0, 0, 0, 0);
							df_STAGE_RECT(1, 0, 0, 0, 0);											break;
		}

		switch (m_tiles[i].obj)
		{
		case OBJ_NONE:		df_STAGE_RECT(2, 0, 0, 0, 0);											break;
		case OBJ_GHOST:		df_STAGE_RECT(2, m_ghost->getMove(), 0, 50, 50);						break;
		default:			df_STAGE_RECT(2, 10, 10, 30, 30);										break;
		}
	}
}

void Scene_Stage::debugRender()
{
	//���׸���

	HPEN Pen1 = CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
	HPEN oldPen1 = (HPEN)SelectObject(m_backBuffer->getMemDC(), Pen1);

	//����ȭ�� ���μ�
	for (int i = 0; i <= df_STAGE_SIZEY; i += df_STAGE_TILE_SIZEY)
	{
		MoveToEx(m_backBuffer->getMemDC(), 0, i, NULL);
		LineTo(m_backBuffer->getMemDC(), df_STAGE_SIZEX, i);
	}

	//����ȭ�� ���μ�
	for (int i = 0; i <= df_STAGE_SIZEX; i += df_STAGE_TILE_SIZEX)
	{
		MoveToEx(m_backBuffer->getMemDC(), i, 0, NULL);
		LineTo(m_backBuffer->getMemDC(), i, df_STAGE_SIZEY);
	}

	SelectObject(m_backBuffer->getMemDC(), oldPen1);
	DeleteObject(Pen1);

	//��Ʈ �׸���

	HPEN Pen2 = CreatePen(PS_NULL, NULL, NULL);
	HPEN oldPen2 = (HPEN)SelectObject(m_backBuffer->getMemDC(), Pen2);

	for (int i = 0; i < df_TILEX* df_TILEY; i++)
	{
		//�ͷ���
		for (int j = 0; j < 2; j++)
		{
			//��ȣ�ۿ�
			if (m_tiles[i].terrain == TR_HOUSE || m_tiles[i].terrain == TR_SKU || m_tiles[i].terrain == TR_FRK || m_tiles[i].terrain == TR_FISH_ALL)
			{
				HBRUSH Brush1 = CreateSolidBrush(RGB(0, 200, 0));
				HBRUSH oldBrush1 = (HBRUSH)SelectObject(m_backBuffer->getMemDC(), Brush1);

				Rectangle(m_backBuffer->getMemDC(), m_tiles[i].rcDebug[j].left, m_tiles[i].rcDebug[j].top, m_tiles[i].rcDebug[j].right, m_tiles[i].rcDebug[j].bottom);

				SelectObject(m_backBuffer->getMemDC(), oldBrush1);
				DeleteObject(Brush1);
			}

			else
			{
				HBRUSH Brush2 = CreateSolidBrush(RGB(200, 0, 0));
				HBRUSH oldBrush2 = (HBRUSH)SelectObject(m_backBuffer->getMemDC(), Brush2);

				Rectangle(m_backBuffer->getMemDC(), m_tiles[i].rcDebug[j].left, m_tiles[i].rcDebug[j].top, m_tiles[i].rcDebug[j].right, m_tiles[i].rcDebug[j].bottom);

				SelectObject(m_backBuffer->getMemDC(), oldBrush2);
				DeleteObject(Brush2);
			}
		}

		//������Ʈ
		if (m_tiles[i].obj != OBJ_NONE)
		{
			HBRUSH Brush3 = CreateSolidBrush(RGB(200, 200, 200));
			HBRUSH oldBrush3 = (HBRUSH)SelectObject(m_backBuffer->getMemDC(), Brush3);

			Rectangle(m_backBuffer->getMemDC(), m_tiles[i].rcDebug[2].left, m_tiles[i].rcDebug[2].top, m_tiles[i].rcDebug[2].right, m_tiles[i].rcDebug[2].bottom);

			SelectObject(m_backBuffer->getMemDC(), oldBrush3);
			DeleteObject(Brush3);
		}
	}

	SelectObject(m_backBuffer->getMemDC(), oldPen2);
	DeleteObject(Pen2);

	//�÷��̾�
	Rectangle(m_backBuffer->getMemDC(), m_player->getPlayerRc().left, m_player->getPlayerRc().top, m_player->getPlayerRc().right, m_player->getPlayerRc().bottom);

	//����Ʈ
	Rectangle(m_backBuffer->getMemDC(), m_player->getEffectRc().left, m_player->getEffectRc().top, m_player->getEffectRc().right, m_player->getEffectRc().bottom);
}

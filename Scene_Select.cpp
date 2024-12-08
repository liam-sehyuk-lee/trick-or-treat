#include "stdafx.h"
#include "Scene_Select.h"

HRESULT Scene_Select::init()
{
	IMAGEMANAGER->addImage("�������", "images/tilemap/bg.bmp", WINSIZEX, WINSIZEY, true);
	m_stageSelect.img = IMAGEMANAGER->addImage("������������", "images/ui/stage_select.bmp", 600, 500, true);
	m_stage[STAGE1].img = IMAGEMANAGER->addFrameImage("��������_1", "images/ui/stage1.bmp", 600, 270, 1, 3, true);
	m_stage[STAGE2].img = IMAGEMANAGER->addFrameImage("��������_2", "images/ui/stage2.bmp", 600, 270, 1, 3, true);
	m_stage[STAGE3].img = IMAGEMANAGER->addFrameImage("��������_3", "images/ui/stage3.bmp", 600, 270, 1, 3, true);

	m_stageSelect.pos.x = -300;
	m_stage[STAGE1].pos.x = m_stage[STAGE2].pos.x = m_stage[STAGE3].pos.x = -300;
	
	for (int i = STAGE1; i <= MAXSTAGE; i++)
	{
		if (g_stageLock[i]) m_stage[i].index = ST_LOCKED;
		else m_stage[i].index = ST_OPENED;
	}

	m_selectedStage = STAGE_NONE;
	m_speed = 20;
	m_count = 0;

	return S_OK;
}

void Scene_Select::release()
{
	//SAFE_DELETE(m_stageSelect.img);
}

void Scene_Select::update()
{
	if (m_stageSelect.pos.x < WINSIZEX / 2 - m_speed)
	{
		m_stageSelect.pos.x += m_speed;
		m_stage[STAGE1].pos.x = m_stage[STAGE2].pos.x = m_stage[STAGE3].pos.x += m_speed;
	}

	else
	{
		m_stageSelect.pos.x = WINSIZEX / 2;
		m_stage[STAGE1].pos.x = m_stage[STAGE2].pos.x = m_stage[STAGE3].pos.x = WINSIZEX / 2;
	}

	//��ġ ������Ʈ
	m_stageSelect.rc = RectMakeCenter(m_stageSelect.pos.x, WINSIZEY / 2, 600, 500);
	m_stage[STAGE1].rc = RectMakeCenter(m_stage[STAGE1].pos.x, 279, 600, 90);
	m_stage[STAGE2].rc = RectMakeCenter(m_stage[STAGE2].pos.x, 379, 600, 90);
	m_stage[STAGE3].rc = RectMakeCenter(m_stage[STAGE3].pos.x, 484, 600, 90);

	if (m_stageSelect.pos.x == WINSIZEX / 2)
	{
		//������
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			for (int i = STAGE1; i <= MAXSTAGE; i++)
			{
				//������� ���� ���������߿��� ���ý�
				if (m_stage[i].index != ST_LOCKED)
				{
					if (PtInRect(&m_stage[i].rc, g_ptMouse))
					{
						SOUNDMANAGER->effectPlay("Ŀ��ȿ����");
						m_selectedStage = (enSTAGE)i;
						m_stage[i].index = ST_SELECTED;
					}
				}
			}
		}

		//�������� ����
		if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
		{
			if (!PtInRect(&m_stage[m_selectedStage].rc, g_ptMouse))
			{
				for (int i = STAGE1; i <= MAXSTAGE; i++)
				{
					if (m_stage[i].index != ST_LOCKED) m_stage[i].index = ST_OPENED;
				}
				m_selectedStage = STAGE_NONE;
			}

			else if (m_selectedStage != STAGE_NONE && m_stage[m_selectedStage].index == ST_SELECTED)
			{
				SOUNDMANAGER->effectPlay("����ȿ����");
				g_stage = m_selectedStage;
				SCENEMANAGER->changeScene("�ε�");
			}
		}
	}
}

void Scene_Select::render()
{
	IMAGEMANAGER->render("�������", getMemDC());

	m_stageSelect.img->render(getMemDC(), m_stageSelect.rc.left, m_stageSelect.rc.top);
	m_stage[STAGE1].img->frameRender(getMemDC(), m_stage[STAGE1].rc.left, m_stage[STAGE1].rc.top, 0, m_stage[STAGE1].index);
	m_stage[STAGE2].img->frameRender(getMemDC(), m_stage[STAGE2].rc.left, m_stage[STAGE2].rc.top, 0, m_stage[STAGE2].index);
	m_stage[STAGE3].img->frameRender(getMemDC(), m_stage[STAGE3].rc.left, m_stage[STAGE3].rc.top, 0, m_stage[STAGE3].index);
}

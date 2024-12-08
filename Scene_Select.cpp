#include "stdafx.h"
#include "Scene_Select.h"

HRESULT Scene_Select::init()
{
	IMAGEMANAGER->addImage("맵툴배경", "images/tilemap/bg.bmp", WINSIZEX, WINSIZEY, true);
	m_stageSelect.img = IMAGEMANAGER->addImage("스테이지선택", "images/ui/stage_select.bmp", 600, 500, true);
	m_stage[STAGE1].img = IMAGEMANAGER->addFrameImage("스테이지_1", "images/ui/stage1.bmp", 600, 270, 1, 3, true);
	m_stage[STAGE2].img = IMAGEMANAGER->addFrameImage("스테이지_2", "images/ui/stage2.bmp", 600, 270, 1, 3, true);
	m_stage[STAGE3].img = IMAGEMANAGER->addFrameImage("스테이지_3", "images/ui/stage3.bmp", 600, 270, 1, 3, true);

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

	//위치 업데이트
	m_stageSelect.rc = RectMakeCenter(m_stageSelect.pos.x, WINSIZEY / 2, 600, 500);
	m_stage[STAGE1].rc = RectMakeCenter(m_stage[STAGE1].pos.x, 279, 600, 90);
	m_stage[STAGE2].rc = RectMakeCenter(m_stage[STAGE2].pos.x, 379, 600, 90);
	m_stage[STAGE3].rc = RectMakeCenter(m_stage[STAGE3].pos.x, 484, 600, 90);

	if (m_stageSelect.pos.x == WINSIZEX / 2)
	{
		//프레임
		if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
		{
			for (int i = STAGE1; i <= MAXSTAGE; i++)
			{
				//잠겨있지 않은 스테이지중에서 선택시
				if (m_stage[i].index != ST_LOCKED)
				{
					if (PtInRect(&m_stage[i].rc, g_ptMouse))
					{
						SOUNDMANAGER->effectPlay("커서효과음");
						m_selectedStage = (enSTAGE)i;
						m_stage[i].index = ST_SELECTED;
					}
				}
			}
		}

		//스테이지 선택
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
				SOUNDMANAGER->effectPlay("선택효과음");
				g_stage = m_selectedStage;
				SCENEMANAGER->changeScene("로딩");
			}
		}
	}
}

void Scene_Select::render()
{
	IMAGEMANAGER->render("맵툴배경", getMemDC());

	m_stageSelect.img->render(getMemDC(), m_stageSelect.rc.left, m_stageSelect.rc.top);
	m_stage[STAGE1].img->frameRender(getMemDC(), m_stage[STAGE1].rc.left, m_stage[STAGE1].rc.top, 0, m_stage[STAGE1].index);
	m_stage[STAGE2].img->frameRender(getMemDC(), m_stage[STAGE2].rc.left, m_stage[STAGE2].rc.top, 0, m_stage[STAGE2].index);
	m_stage[STAGE3].img->frameRender(getMemDC(), m_stage[STAGE3].rc.left, m_stage[STAGE3].rc.top, 0, m_stage[STAGE3].index);
}

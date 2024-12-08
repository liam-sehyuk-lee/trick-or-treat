#include "stdafx.h"
#include "Scene_Title.h"

HRESULT Scene_Title::init()
{
	SOUNDMANAGER->play("인게임음악", 1.0f);

	m_image = IMAGEMANAGER->addFrameImage("타이틀배경", "images/ui/title.bmp", WINSIZEX * 3, WINSIZEY, 3, 1, true);

	m_playRc = RectMake(40, 670, 480, 70);
	m_mapRc = RectMake(1100, 15, 355, 55);

	m_select = TT_NONE;

	return S_OK;
}

void Scene_Title::release()
{
}

void Scene_Title::update()
{
	//프레임
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&m_playRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("커서효과음");
			m_select = TT_PLAY;
		}

		if (PtInRect(&m_mapRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("커서효과음");
			m_select = TT_MAP;
		}
	}

	//이동
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		if (!PtInRect(&m_playRc, g_ptMouse) && !PtInRect(&m_mapRc, g_ptMouse)) m_select = TT_NONE;

		if (PtInRect(&m_playRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("선택효과음");
			SCENEMANAGER->changeScene("게임방법");
		}

		if (PtInRect(&m_mapRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("선택효과음");
			SCENEMANAGER->changeScene("맵툴");
		}
	}
}

void Scene_Title::render()
{
	m_image->frameRender(getMemDC(), 0, 0, m_select, 0);
}

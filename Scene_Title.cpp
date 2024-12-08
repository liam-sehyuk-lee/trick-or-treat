#include "stdafx.h"
#include "Scene_Title.h"

HRESULT Scene_Title::init()
{
	SOUNDMANAGER->play("�ΰ�������", 1.0f);

	m_image = IMAGEMANAGER->addFrameImage("Ÿ��Ʋ���", "images/ui/title.bmp", WINSIZEX * 3, WINSIZEY, 3, 1, true);

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
	//������
	if (KEYMANAGER->isOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&m_playRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("Ŀ��ȿ����");
			m_select = TT_PLAY;
		}

		if (PtInRect(&m_mapRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("Ŀ��ȿ����");
			m_select = TT_MAP;
		}
	}

	//�̵�
	if (KEYMANAGER->isOnceKeyUp(VK_LBUTTON))
	{
		if (!PtInRect(&m_playRc, g_ptMouse) && !PtInRect(&m_mapRc, g_ptMouse)) m_select = TT_NONE;

		if (PtInRect(&m_playRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("����ȿ����");
			SCENEMANAGER->changeScene("���ӹ��");
		}

		if (PtInRect(&m_mapRc, g_ptMouse))
		{
			SOUNDMANAGER->effectPlay("����ȿ����");
			SCENEMANAGER->changeScene("����");
		}
	}
}

void Scene_Title::render()
{
	m_image->frameRender(getMemDC(), 0, 0, m_select, 0);
}

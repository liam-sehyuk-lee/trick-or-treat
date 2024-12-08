#include "stdafx.h"
#include "Scene_Loading.h"

HRESULT Scene_Loading::init()
{
	IMAGEMANAGER->addImage("�������", "images/tilemap/bg.bmp", WINSIZEX, WINSIZEY, true);
	m_loadingBack.img = IMAGEMANAGER->addImage("�ε���", "images/ui/loading_back.bmp", 600, 200, true);
	
	m_loadingBack.pos.x = -300;
	m_speed = 20;
	m_count = 0;

	//�ε��� �ʱ�ȭ
	m_loadingBar = new progressBar;
	m_loadingBar->init("images/ui/loading_front.bmp", "images/ui/loading_back.bmp",
		WINSIZEX / 2 - 300, WINSIZEY / 2 - 100, 600, 200);
	
	m_loadingBar->setGauge(0, 0);

	//�ε��� ������ �ʱ�ȭ
	m_currentGauge = 0;

	return S_OK;
}

void Scene_Loading::release()
{
	//SAFE_DELETE(m_loadingBack.img);
	SAFE_DELETE(m_loadingBar);
}

void Scene_Loading::update()
{
	if (m_loadingBack.pos.x < WINSIZEX / 2 - m_speed) m_loadingBack.pos.x += m_speed;
	
	else
	{
		m_loadingBack.pos.x = WINSIZEX / 2;

		//�ε���
		m_loadingBar->update();
		m_loadingBar->setGauge(m_currentGauge, 100);

		if (m_currentGauge < 100) m_currentGauge++;
		else
		{
			m_currentGauge = 100;

			switch (g_stage)
			{
			case STAGE1: SCENEMANAGER->changeScene("��������1"); break;
			case STAGE2: SCENEMANAGER->changeScene("��������2"); break;
			case STAGE3: SCENEMANAGER->changeScene("��������3"); break;
			}
		}
	}
	
	//��ġ ������Ʈ
	m_loadingBack.rc = RectMakeCenter(m_loadingBack.pos.x, WINSIZEY / 2, 600, 200);
}

void Scene_Loading::render()
{
	IMAGEMANAGER->render("�������", getMemDC());

	if (m_loadingBack.pos.x < WINSIZEX / 2 - m_speed)
	{
		m_loadingBack.img->render(getMemDC(), m_loadingBack.rc.left, m_loadingBack.rc.top);
	}

	else m_loadingBar->render();
}

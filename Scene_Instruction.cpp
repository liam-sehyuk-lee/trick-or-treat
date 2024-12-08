#include "stdafx.h"
#include "Scene_Instruction.h"

HRESULT Scene_Instruction::init()
{
	IMAGEMANAGER->addImage("맵툴배경", "images/tilemap/bg.bmp", WINSIZEX, WINSIZEY, true);
	m_howTo.img = IMAGEMANAGER->addImage("플레이방법", "images/ui/how_to_play.bmp", 600, 500, true);
	m_control.img = IMAGEMANAGER->addImage("컨트롤방법", "images/ui/game_controls.bmp", 600, 300, true);

	m_howTo.pos.x = m_control.pos.x = -300;
	m_speed = 20;
	m_count = 0;

	return S_OK;
}

void Scene_Instruction::release()
{
	//SAFE_DELETE(m_howTo.img);
	//SAFE_DELETE(m_control.img);
}

void Scene_Instruction::update()
{
	if (m_howTo.pos.x < WINSIZEX / 2 - m_speed) m_howTo.pos.x += m_speed;
	else { m_howTo.pos.x = WINSIZEX / 2;

		m_count++;
		if (m_count >= 100)
		{
			if (m_control.pos.x < WINSIZEX / 2 - m_speed) m_control.pos.x += m_speed;
			else m_control.pos.x = WINSIZEX / 2;
		}
	}
	
	if (m_count >= 250) SCENEMANAGER->changeScene("선택화면");

	//위치 업데이트
	m_howTo.rc = RectMakeCenter(m_howTo.pos.x, WINSIZEY / 2, 600, 500);
	m_control.rc = RectMakeCenter(m_control.pos.x, WINSIZEY / 2, 600, 300);
}

void Scene_Instruction::render()
{
	IMAGEMANAGER->render("맵툴배경", getMemDC());

	if (m_count < 100) m_howTo.img->render(getMemDC(), m_howTo.rc.left, m_howTo.rc.top);
	m_control.img->render(getMemDC(), m_control.rc.left, m_control.rc.top);
}

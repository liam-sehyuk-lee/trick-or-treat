#include "stdafx.h"
#include "Object_Player.h"

HRESULT Object_Player::init(int startPosX, int startPosY)
{
	m_effect = new Object_Effect;
	m_effect->init();

	char str[2][128];
	for (int i = 1; i <= df_MAXCHAR; i++)
	{
		sprintf_s(str[0], "ĳ����%d", i);
		sprintf_s(str[1], "images/character/char%d.bmp", i);
		IMAGEMANAGER->addFrameImage(str[0], str[1], 630, 280, 9, 4, true);
	}

	IMAGEMANAGER->addFrameImage("ĳ����5-1", "images/character/char5-1.bmp", 490, 280, 7, 4, true);

	IMAGEMANAGER->addFrameImage("ĳ����2_��ų1", "images/skill/char2_skill_1.bmp", 140, 70, 2, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����2_��ų2", "images/skill/char2_skill_2.bmp", 140, 70, 2, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����2_��ų3", "images/skill/char2_skill_3.bmp", 140, 70, 2, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����2_��ų4", "images/skill/char2_skill_4.bmp", 140, 70, 2, 1, true);

	IMAGEMANAGER->addFrameImage("ĳ����3_��ų1", "images/skill/char3_skill_1.bmp", 490, 70, 7, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����3_��ų2", "images/skill/char3_skill_2.bmp", 350, 70, 5, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����3_��ų3", "images/skill/char3_skill_3.bmp", 560, 70, 8, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����3_��ų4", "images/skill/char3_skill_4.bmp", 560, 70, 8, 1, true);

	IMAGEMANAGER->addFrameImage("ĳ����4_��ų1", "images/skill/char4_skill_1.bmp", 350, 70, 5, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����4_��ų2", "images/skill/char4_skill_2.bmp", 420, 70, 6, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����4_��ų3", "images/skill/char4_skill_3.bmp", 350, 70, 6, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����4_��ų4", "images/skill/char4_skill_4.bmp", 350, 70, 6, 1, true);

	IMAGEMANAGER->addFrameImage("ĳ����5_��ų1", "images/skill/char5_skill_1.bmp", 210, 70, 3, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����5_��ų2", "images/skill/char5_skill_2.bmp", 210, 70, 3, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����5_��ų3", "images/skill/char5_skill_3.bmp", 280, 70, 4, 1, true);
	IMAGEMANAGER->addFrameImage("ĳ����5_��ų4", "images/skill/char5_skill_4.bmp", 280, 70, 4, 1, true);


	m_player.img = IMAGEMANAGER->findImage("ĳ����1");

	m_player.pos.x = startPosX - 11;
	m_player.pos.y = startPosY - 11;

	m_player.index, m_player.count = 0;
	m_player.speed = 5;

	m_pos = POS_FRONT;
	m_charType = SKULL;
	m_charState = CHAR_IDLE;

	return S_OK;
}

void Object_Player::release()
{
	SAFE_DELETE(m_effect);
}

void Object_Player::update()
{
	if (m_charState != CHAR_SPELL || m_charType == DRACULA) move();
	if (m_charState != CHAR_SPELL) change();
	
	frame();
	m_effect->update(m_player.rcPos, m_pos, m_charType);
}

void Object_Player::render(HDC hdc)
{
	m_player.img->frameRender(hdc, m_player.pos.x, m_player.pos.y, m_player.img->getFrameX(), m_player.img->getFrameY());
	
	if (m_charState == CHAR_SPELL)	m_effect->render(hdc);
}

void Object_Player::move()
{
	//���� ĳ���� ��Ȳ
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE))	//��ų ����
	{
		if (m_charType == DRACULA) SOUNDMANAGER->effectPlay("ĳ����2_ȿ����");

		if (m_charState == CHAR_SPELL && m_charType == DRACULA) m_charState = CHAR_IDLE;
		else if (m_charType == FISHMAN) m_charState = CHAR_IDLE;
		else m_charState = CHAR_SPELL;

		m_player.index = 0;
		m_effect->setFrame();
		skill();
	}

	else if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		m_player.pos.y -= m_player.speed;		
		m_pos = POS_BACK;

		if (m_player.rc.top < 20) collision();
	}

	else if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		m_player.pos.y += m_player.speed;
		m_pos = POS_FRONT;


		if (m_player.rc.bottom > df_STAGE_SIZEY - 20) collision();
	}

	else if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		m_player.pos.x -= m_player.speed;
		m_pos = POS_LEFT;

		if (m_player.rc.left < 20) collision();
	}

	else if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		m_player.pos.x += m_player.speed;
		m_pos = POS_RIGHT;

		if (m_player.rc.right > df_STAGE_SIZEX - 20) collision();
	}

	//��ġ ������Ʈ
	m_player.rcPos.x = m_player.pos.x + 35;
	m_player.rcPos.y = m_player.pos.y + 35;
	m_player.rc = RectMakeCenter(m_player.rcPos.x, m_player.rcPos.y, 20, 20);
}

void Object_Player::skill()
{
	if (m_charType != SKULL && m_charState == CHAR_SPELL)
	{
		char str[128];
		sprintf_s(str, "ĳ����%d_��ų%d", m_charType + 1, m_pos + 1);
		m_player.img = IMAGEMANAGER->findImage(str);
	}

	if (m_charType != DRACULA && m_charType != FISHMAN)
	{
		char str1[128];
		sprintf_s(str1, "ĳ����%d_ȿ����", m_charType + 1);
		SOUNDMANAGER->effectPlay(str1);
	}
}

void Object_Player::collision()
{
	switch (m_pos)
	{
	case POS_FRONT:		m_player.pos.y -= m_player.speed;	break;
	case POS_BACK:		m_player.pos.y += m_player.speed;	break;
	case POS_LEFT:		m_player.pos.x += m_player.speed;	break;
	case POS_RIGHT:		m_player.pos.x -= m_player.speed;	break;
	}
}

void Object_Player::change()
{
	//ĳ���� ��� ��ȭ
	char str[128];
	sprintf_s(str, "ĳ����%d", m_charType + 1);
	m_player.img = IMAGEMANAGER->findImage(str);
}

void Object_Player::inWater()
{
	m_player.img = IMAGEMANAGER->findImage("ĳ����5-1");
	m_player.img->setFrameY(m_pos);
	m_player.img->setFrameX(m_player.index);
}

void Object_Player::frame()
{
	//ĳ���� ��Ȳ�� ���� ������ ��ȭ
			//�Ϲ�								����� �������϶�
	if (m_charState == CHAR_IDLE || (m_charState == CHAR_SPELL && m_charType == DRACULA))
	{
		m_player.img->setFrameY(m_pos);
		if (m_charType == DRACULA) skill();

		//�����̰� ������
		if (KEYMANAGER->isStayKeyDown(VK_UP) || KEYMANAGER->isStayKeyDown(VK_DOWN) ||
			KEYMANAGER->isStayKeyDown(VK_LEFT) || KEYMANAGER->isStayKeyDown(VK_RIGHT))
		{
			m_player.count++;
			if (m_player.count % 5 == 0)
			{
				m_player.count = 0;
				m_player.index++;

				if (m_player.index > m_player.img->getMaxFrameX()) m_player.index = 1 - m_charState;
				m_player.img->setFrameX(m_player.index);
			}
		}

		//�������� ������
		else m_player.img->setFrameX(0);
	}
	
	else //��ų ����
	{
		m_player.count++;
		if (m_player.count % 5 == 0)
		{
			m_player.count = 0;
			m_player.index++;
			m_player.img->setFrameX(m_player.index);

			if (m_player.index > m_player.img->getMaxFrameX()) m_charState = CHAR_IDLE;
		}
	}	
}
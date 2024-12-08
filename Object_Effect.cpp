#include "stdafx.h"
#include "Object_Effect.h"

HRESULT Object_Effect::init()
{
	m_effect[EFT_SKULL].img = IMAGEMANAGER->addFrameImage("캐릭터1_이펙트", "images/effect/char1_effect.bmp", 126, 11, 6, 1, true);
	m_effect[EFT_WITCH].img = IMAGEMANAGER->addFrameImage("캐릭터4_이펙트", "images/effect/char4_effect.bmp", 490, 70, 7, 1, true);
	m_effect[EFT_FISH].img  = IMAGEMANAGER->addFrameImage("캐릭터5_이펙트", "images/effect/char5_effect.bmp", 700, 70, 10, 1, true);

	for (int i = 1; i < df_MAXEFFECT; i++) m_effect[i].index = m_effect[i].count = 0;

	m_skullSkill.count = m_skullSkill.index = 0;

	m_curEffect = EFT_NONE;

	return S_OK;
}

void Object_Effect::release()
{
}

void Object_Effect::update(POINT playerPos, enPOSITION pos, enCHARACTER_TYPE charType)
{						   
	switch (charType)
	{
	case DRACULA:
		m_effectPos.x = playerPos.x;
		m_effectPos.y = playerPos.y;
		break;

	case SKULL: case FRANKEN: case WITCH: case FISHMAN:
		switch (pos)
		{
		case POS_FRONT:
			m_effectPos.x = playerPos.x;
			m_effectPos.y = playerPos.y + df_STAGE_TILE_SIZEY;
			break;

		case POS_BACK:
			m_effectPos.x = playerPos.x;
			m_effectPos.y = playerPos.y - df_STAGE_TILE_SIZEY;
			break;

		case POS_LEFT:
			m_effectPos.x = playerPos.x - df_STAGE_TILE_SIZEX;
			m_effectPos.y = playerPos.y;
			break;

		case POS_RIGHT:
			m_effectPos.x = playerPos.x + df_STAGE_TILE_SIZEX;
			m_effectPos.y = playerPos.y;
			break;
		}
	}

	m_effectRc = RectMakeCenter(m_effectPos.x, m_effectPos.y, 20, 20);

	frame();

	m_playerPos = playerPos;
	m_charType = charType;
}

void Object_Effect::render(HDC hdc)
{
	switch (m_charType)
	{
	case SKULL:
		m_curEffect = EFT_SKULL;
		for (int i = 0; i < 4; i++)
		{
			m_effect[EFT_SKULL].img->frameRender(hdc, m_skullSkill.rc[i].left, m_skullSkill.rc[i].top, m_effect[EFT_SKULL].img->getFrameX(), m_effect[EFT_SKULL].img->getFrameY());
		}
		break;

	case WITCH:
		m_curEffect = EFT_WITCH;
		m_effect[EFT_WITCH].img->frameRender(hdc, m_effectPos.x - 35, m_effectPos.y - 35, m_effect[EFT_WITCH].img->getFrameX(), m_effect[EFT_WITCH].img->getFrameY());
		break;

	case FISHMAN:
		m_curEffect = EFT_FISH;
		m_effect[EFT_FISH].img->frameRender(hdc, m_playerPos.x - 35, m_playerPos.y - 35, m_effect[EFT_FISH].img->getFrameX(), m_effect[EFT_FISH].img->getFrameY());
		break;

	default:	m_curEffect = EFT_NONE;		break;
	}
}

void Object_Effect::frame()
{
	//해골 스킬 렉트
	if (m_curEffect == EFT_SKULL)
	{
		m_skullSkill.count++;
		if (m_skullSkill.count % 5 == 0)
		{
			m_skullSkill.count = 0;
			m_skullSkill.index++;

			for (int i = 0; i < 4; i++)
			{
				m_skullSkill.rc[i] = RectMakeCenter(
					m_playerPos.x + df_RADIUS * (cos(PI_2 * i - PI_12 * m_skullSkill.index)),
					m_playerPos.y - df_RADIUS * (sin(PI_2 * i - PI_12 * m_skullSkill.index)), 20, 20);
			}
		}
	}
	
	for (int i = 1; i < df_MAXEFFECT; i++)
	{
		m_effect[i].count++;
		if (m_effect[i].count % 5 == 0)
		{
			m_effect[i].count = 0;
			m_effect[i].index++;

			m_effect[i].img->setFrameX(m_effect[i].index);

			if (m_effect[i].index > m_effect[i].img->getMaxFrameX())
			{
				m_effect[i].count = m_effect[i].img->getMaxFrameX();
			}
		}
	}
}

void Object_Effect::setFrame()
{
	for (int i = 1; i < df_MAXEFFECT; i++) m_effect[i].index = m_effect[i].count = 0;
}

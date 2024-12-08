#include "stdafx.h"
#include "Object_Ghost.h"

HRESULT Object_Ghost::init()
{
	m_image = IMAGEMANAGER->addFrameImage("ghost", "images/object/ghost.bmp", 152, 94, 4, 2, true);

	m_count = m_index = 0;
	m_move = 0;
	m_pos = POS_LEFT;

	return S_OK;
}

void Object_Ghost::release()
{
}

void Object_Ghost::update()
{
	if (!getChanged()) move();
	frame();
}

void Object_Ghost::render(HDC hdc, int x, int y)
{
	m_image->frameRender(hdc, x + m_move, y);
}

void Object_Ghost::move()
{
	switch (m_pos)
	{
	case POS_LEFT:

		m_move -= 3;
		if (m_move < -200) m_pos = POS_RIGHT;
		break;

	case POS_RIGHT:

		m_move += 3;
		if (m_move > 200) m_pos = POS_LEFT;
		break;
	}
}

void Object_Ghost::collision()
{
	switch (m_pos)
	{
	case POS_LEFT:		m_pos = POS_RIGHT;		break;
	case POS_RIGHT:		m_pos = POS_LEFT;		break;
	}
}

void Object_Ghost::frame()
{
	switch (m_pos)
	{
	case POS_LEFT:
		m_image->setFrameX(POS_LEFT);
		break;

	case POS_RIGHT:
		m_image->setFrameX(POS_RIGHT);
		break;
	}
}
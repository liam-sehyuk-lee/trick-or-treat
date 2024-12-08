#include "stdafx.h"
#include "Object_Lock.h"

HRESULT Object_Lock::init()
{
	m_lock[LOCK1].img = IMAGEMANAGER->addFrameImage("lock1", "images/object/lock1.bmp", 288, 48, 6, 1, true);
	m_lock[LOCK2].img = IMAGEMANAGER->addFrameImage("lock2", "images/object/lock2.bmp", 288, 48, 6, 1, true);

	for (int i = 1; i < df_MAXLOCK; i++)
	{
		m_lock[i].count = m_lock[i].index = 0;
		m_lock[i].on = false;
	}

	return S_OK;
}

void Object_Lock::release()
{
}

void Object_Lock::update()
{
	frame();
}

void Object_Lock::render(HDC hdc, enLOCK lock, int x, int y)
{
	m_lock[lock].img->frameRender(hdc, x, y);
}

void Object_Lock::frame()
{
	for (int i = 1; i < df_MAXLOCK; i++)
	{
		if (m_lock[i].on)
		{
			m_lock[i].count++;
			if (m_lock[i].count % 5 == 0)
			{
				m_lock[i].count = 0;
				m_lock[i].index++;

				m_lock[i].img->setFrameX(m_lock[i].index);

				if (m_lock[i].index > m_lock[i].img->getMaxFrameX())
				{
					m_lock[i].index = m_lock[i].img->getMaxFrameX();
				}
			}
		}
	}
}

void Object_Lock::setFrame(enLOCK lock)
{
	if (!m_lock[lock].on)
	{
		m_lock[lock].count = m_lock[lock].index = 0;
		m_lock[lock].on = true;
	}
}
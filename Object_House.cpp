#include "stdafx.h"
#include "Object_House.h"

HRESULT Object_House::init()
{
	m_house[HOUSE1] = IMAGEMANAGER->addFrameImage("house1", "images/object/house1.bmp", 672, 336, 2, 1, true);
	m_house[HOUSE2] = IMAGEMANAGER->addFrameImage("house2", "images/object/house2.bmp", 480, 192, 2, 1, true);
	m_house[HOUSE3] = IMAGEMANAGER->addFrameImage("house3", "images/object/house3.bmp", 480, 192, 2, 1, true);
	m_house[HOUSE4] = IMAGEMANAGER->addFrameImage("house4", "images/object/house4.bmp", 288, 336, 2, 1, true);
	m_house[HOUSE5] = IMAGEMANAGER->addFrameImage("house5", "images/object/house5.bmp", 288, 288, 2, 1, true);

	return S_OK;
}

void Object_House::release()
{
}

void Object_House::update()
{
}

void Object_House::render(HDC hdc, enHOUSE house, int x, int y)
{
	switch (house)
	{
	case HOUSE1:	m_house[house]->frameRender(hdc, x, y);			break;
	case HOUSE2:	m_house[house]->frameRender(hdc, x, y);			break;
	case HOUSE3:	m_house[house]->frameRender(hdc, x, y);			break;
	case HOUSE4:	m_house[house]->frameRender(hdc, x, y);			break;
	case HOUSE5:	m_house[house]->frameRender(hdc, x, y);			break;
	}
}

#include "stdafx.h"
#include "Object_Item.h"

HRESULT Object_Item::init()
{
	m_item[IN_CANDY].img =		IMAGEMANAGER->addFrameImage("캔디", "images/object/candy.bmp", 189, 17, 7, 1, true);
	m_item[IN_COIN].img =		IMAGEMANAGER->addFrameImage("코인", "images/object/coin.bmp", 64, 16, 4, 1, true);
	m_item[IN_SUPER_COIN].img =	IMAGEMANAGER->addFrameImage("슈퍼코인", "images/object/super_coin.bmp", 150, 24, 6, 1, true);
	m_item[IN_ICON].img =		IMAGEMANAGER->addFrameImage("아이콘", "images/object/icon.bmp", 174, 215, 6, 5, true);

	IMAGEMANAGER->addImage("그림자", "images/object/object_shade.bmp", 19, 10, true);

	for (int i = 0; i < df_MAXITEM; i++) m_item[i].count = m_item[i].index = 0;

	return S_OK;
}

void Object_Item::release()
{
}

void Object_Item::update()
{
	frame();
}

void Object_Item::render(HDC hdc, enINGAME_ITEM item, int x, int y, enICON iconType)
{
	switch (item)
	{
	case IN_CANDY: 			
		m_item[item].img->frameRender(hdc, x + 10.5, y + 5);
		IMAGEMANAGER->render("그림자", hdc, x + 14.5, y + 38);
		break;

	case IN_COIN:			
		m_item[item].img->frameRender(hdc, x + 16, y + 5);
		IMAGEMANAGER->render("그림자", hdc, x + 14.5, y + 38);
		break;

	case IN_SUPER_COIN:
		m_item[item].img->frameRender(hdc, x + 11.5, y + 4);
		IMAGEMANAGER->render("그림자", hdc, x + 14.5, y + 38);
		break;

	case IN_ICON:
		m_item[item].img->setFrameY(iconType);
		m_item[item].img->frameRender(hdc, x + 9.5, y + 2.5);
		break;
	}
}

void Object_Item::frame()
{
	for (int i = 0; i < df_MAXITEM; i++)
	{
		m_item[i].count++;
		if (m_item[i].count % 10 == 0)
		{
			m_item[i].count = 0;
			m_item[i].index++;

			if (m_item[i].index > m_item[i].img->getMaxFrameX()) m_item[i].index = 0;
			m_item[i].img->setFrameX(m_item[i].index);
		}
	}
}

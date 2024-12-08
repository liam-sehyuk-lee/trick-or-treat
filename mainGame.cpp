#include "stdafx.h"
#include "mainGame.h"

HRESULT mainGame::init()
{
	gameNode::init(true);

	SOUNDMANAGER->init();

	SCENEMANAGER->init();
	SCENEMANAGER->changeScene("타이틀");
	//SCENEMANAGER->changeScene("스테이지3");
	return S_OK;
}

void mainGame::release()
{
	gameNode::release();
	SOUNDMANAGER->release();
	SCENEMANAGER->release();
}

void mainGame::update()
{
	gameNode::update();
	SOUNDMANAGER->update();
	SCENEMANAGER->update();
}

void mainGame::render()
{
	//흰색 비트맵
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//====================================================

	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());
	//=====================================================
	//백버퍼의 내용을 HDC에 그린다.(지우지마!!)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}










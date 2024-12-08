#include "stdafx.h"
#include "mainGame.h"

HRESULT mainGame::init()
{
	gameNode::init(true);

	SOUNDMANAGER->init();

	SCENEMANAGER->init();
	SCENEMANAGER->changeScene("Ÿ��Ʋ");
	//SCENEMANAGER->changeScene("��������3");
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
	//��� ��Ʈ��
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, WHITENESS);
	//====================================================

	SCENEMANAGER->render();

	TIMEMANAGER->render(getMemDC());
	//=====================================================
	//������� ������ HDC�� �׸���.(��������!!)
	this->getBackBuffer()->render(getHDC(), 0, 0);
}










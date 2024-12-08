#pragma once
#include "Scene.h"
#include"progressBar.h"

class Scene_Loading : public Scene
{
private:

	stScene m_loadingBack;

	progressBar* m_loadingBar;
	int m_currentGauge;

public:

	HRESULT init();
	void release();
	void update();
	void render();

public:
	Scene_Loading() {};
	~Scene_Loading() {};
};


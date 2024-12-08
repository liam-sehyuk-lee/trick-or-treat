#pragma once
#include "Scene.h"

enum enTITLE { TT_NONE, TT_PLAY, TT_MAP };

class Scene_Title : public Scene
{
private:

	image* m_image;
	RECT m_mapRc, m_playRc;
	enTITLE m_select;

public:

	HRESULT init();
	void release();
	void update();
	void render();

public:
	Scene_Title() {};
	~Scene_Title() {};
};


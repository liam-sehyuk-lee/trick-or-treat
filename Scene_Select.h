#pragma once
#include"Scene.h"

#define MAXSTAGE 4

enum enSTAGE;

class Scene_Select : public Scene
{
private:

	stScene m_stageSelect;
	stScene m_stage[MAXSTAGE];

	enSTAGE m_selectedStage;

public:

	HRESULT init();
	void release();
	void update();
	void render();

public:
	Scene_Select() {};
	~Scene_Select() {};
};


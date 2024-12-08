#pragma once
#include "Scene.h"

class Scene_Instruction : public Scene
{
private:

	stScene m_howTo, m_control;

public:

	HRESULT init();
	void release();
	void update();
	void render();

public:
	Scene_Instruction() {};
	~Scene_Instruction() {};
};


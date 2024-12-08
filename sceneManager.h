#pragma once
#include"singletonBase.h"
#include"Scene_Title.h"
#include"Scene_Instruction.h"
#include"Scene_Loading.h"
#include"Scene_Select.h"
#include"Scene_Maptool.h"
#include"Scene_Stage.h"

class gameNode;

class sceneManager : public singletonBase<sceneManager>
{
private:

	Scene_Title* m_title;
	Scene_Instruction* m_instruction;
	Scene_Loading* m_loading;
	Scene_Select* m_select;
	Scene_Maptool* m_maptool;
	Scene_Stage* m_stage1, *m_stage2, *m_stage3;

private:

	typedef map<string, gameNode*> mSceneList;
	typedef map<string, gameNode*>::iterator miSceneList;

private:
	//ÇöÀç¾À
	static gameNode* _currentScene;
	//¾À¸ñ·Ï
	mSceneList _mSceneList;

public:
	sceneManager();
	~sceneManager();

	HRESULT init();
	void release();
	void update();
	void render();
	//¾ÀÃß°¡ÇÏ±â
	gameNode* addScene(string sceneName, gameNode* scene);

	//¾Àº¯°æ(ÇöÀç¾ÀÀ¸·Î)
	HRESULT changeScene(string sceneName);


};


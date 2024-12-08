#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

sceneManager::sceneManager()
{
}
sceneManager::~sceneManager()
{
}

//현재씬을 널값으로 초기화 하기
gameNode* sceneManager::_currentScene = nullptr;
HRESULT sceneManager::init()
{
	//@@@@@@@@@@@@@@신 여기에 추가
	m_title = new Scene_Title;
	m_instruction = new Scene_Instruction;
	m_loading = new Scene_Loading;
	m_select = new Scene_Select;
	m_maptool = new Scene_Maptool;
	m_stage1 = new Scene_Stage;
	m_stage2 = new Scene_Stage;
	m_stage3 = new Scene_Stage;

	addScene("타이틀", m_title);
	addScene("게임방법", m_instruction);
	addScene("로딩", m_loading);
	addScene("선택화면", m_select);
	addScene("맵툴", m_maptool);
	addScene("스테이지1", m_stage1);
	addScene("스테이지2", m_stage2);
	addScene("스테이지3", m_stage3);

	return S_OK;
}

void sceneManager::release()
{
	//@@@@@@@@@@@@@@신 지워주기
	SAFE_DELETE(m_title);
	SAFE_DELETE(m_instruction);
	SAFE_DELETE(m_loading);
	SAFE_DELETE(m_select);
	SAFE_DELETE(m_maptool);
	SAFE_DELETE(m_stage1);
	SAFE_DELETE(m_stage2);
	SAFE_DELETE(m_stage3);

	miSceneList iter = _mSceneList.begin();
	for (iter; iter != _mSceneList.end();)
	{
		//삭제
		if (iter->second != NULL)
		{
			if (iter->second == _currentScene)iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	_mSceneList.clear();
}

void sceneManager::update()
{
	if (_currentScene)
	{
		_currentScene->update();
	}
}

void sceneManager::render()
{
	if (_currentScene)_currentScene->render();
}
gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{

	if (!scene) return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return nullptr;
}

HRESULT sceneManager::changeScene(string sceneName)
{

	miSceneList find = _mSceneList.find(sceneName);

	//못찾으면 E_FAIL
	if (find == _mSceneList.end())return E_FAIL;

	//바꾸려는 씬이 현재씬과 같아도 E_FAIL
	if (find->second == _currentScene)return E_FAIL;

	//여기까지 왔다는건 문제가 없다. 즉 씬을 초기화 하고 변경해라.
	if (SUCCEEDED(find->second->init()))
	{
		//혹시 기존에 씬이 있다면 릴리즈
		if (_currentScene)_currentScene->release();
		_currentScene = find->second;
		return S_OK;
	}
	return E_FAIL;
}

#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

sceneManager::sceneManager()
{
}
sceneManager::~sceneManager()
{
}

//������� �ΰ����� �ʱ�ȭ �ϱ�
gameNode* sceneManager::_currentScene = nullptr;
HRESULT sceneManager::init()
{
	//@@@@@@@@@@@@@@�� ���⿡ �߰�
	m_title = new Scene_Title;
	m_instruction = new Scene_Instruction;
	m_loading = new Scene_Loading;
	m_select = new Scene_Select;
	m_maptool = new Scene_Maptool;
	m_stage1 = new Scene_Stage;
	m_stage2 = new Scene_Stage;
	m_stage3 = new Scene_Stage;

	addScene("Ÿ��Ʋ", m_title);
	addScene("���ӹ��", m_instruction);
	addScene("�ε�", m_loading);
	addScene("����ȭ��", m_select);
	addScene("����", m_maptool);
	addScene("��������1", m_stage1);
	addScene("��������2", m_stage2);
	addScene("��������3", m_stage3);

	return S_OK;
}

void sceneManager::release()
{
	//@@@@@@@@@@@@@@�� �����ֱ�
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
		//����
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

	//��ã���� E_FAIL
	if (find == _mSceneList.end())return E_FAIL;

	//�ٲٷ��� ���� ������� ���Ƶ� E_FAIL
	if (find->second == _currentScene)return E_FAIL;

	//������� �Դٴ°� ������ ����. �� ���� �ʱ�ȭ �ϰ� �����ض�.
	if (SUCCEEDED(find->second->init()))
	{
		//Ȥ�� ������ ���� �ִٸ� ������
		if (_currentScene)_currentScene->release();
		_currentScene = find->second;
		return S_OK;
	}
	return E_FAIL;
}

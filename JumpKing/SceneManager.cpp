#include "pch.h"
#include "SceneManager.h"

#include "StartScene.h"
#include "StageScene.h"

UINT SceneManager::g_stageNumber = 1;

SceneManager::SceneManager() 
	: 
	p_scenes{},
	p_curScene(nullptr)
{

}

SceneManager::~SceneManager()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(SCENE_TYPE::END); ++i)
	{
		if (p_scenes[i] != nullptr)
		{
			delete p_scenes[i];
		}
	}
}

void SceneManager::Init()
{
	// Scene 생성
	p_scenes[static_cast<unsigned int>(SCENE_TYPE::STAGE_01)] = new StartScene;
	p_scenes[static_cast<unsigned int>(SCENE_TYPE::STAGE_01)]->SetName(L"StartScene");


	// 현재 씬 지정
	p_curScene = p_scenes[static_cast<unsigned int>(SCENE_TYPE::STAGE_01)];
	p_curScene->Enter();
}

void SceneManager::Update()
{
	p_curScene->Update();
	p_curScene->FinalUpdate();
}

void SceneManager::Render(HDC sceneDC)
{
	p_curScene->Render(sceneDC);
}

void SceneManager::ChangeRealScene(SCENE_TYPE sceneType)
{
	p_curScene->Exit();

	p_curScene = p_scenes[static_cast<UINT>(sceneType)];

	p_curScene->Enter();
}


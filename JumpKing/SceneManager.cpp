#include "pch.h"
#include "SceneManager.h"

#include "StartScene.h"

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
	p_scenes[static_cast<unsigned int>(SCENE_TYPE::START)] = new StartScene;
	p_scenes[static_cast<unsigned int>(SCENE_TYPE::START)]->SetName(L"StartScene");


	// 현재 씬 지정
	p_curScene = p_scenes[static_cast<unsigned int>(SCENE_TYPE::START)];
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


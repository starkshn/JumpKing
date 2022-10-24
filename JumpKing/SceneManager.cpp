#include "pch.h"
#include "SceneManager.h"

#include "StartScene.h"
#include "StageScene.h"

#include "Object.h"

UINT SceneManager::g_stageNumber = 0;
UINT SceneManager::g_staticStage = 0;
UINT SceneManager::g_prevStageNum = 0;

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
			p_scenes[i] = nullptr;
		}
	}
}

void SceneManager::Init()
{
	// Scene 생성
	for (UINT i = static_cast<unsigned int>(SCENE_TYPE::STAGE_01); i < static_cast<unsigned int>(SCENE_TYPE::END); ++i)
	{
		string str = to_string(i);
		wstring strP = wstring(str.begin(), str.end());

		p_scenes[static_cast<unsigned int>((SCENE_TYPE)(i)) - 1] = new StageScene;
		p_scenes[static_cast<unsigned int>((SCENE_TYPE)(i)) - 1]->SetSceneName(L"STAGE_" + strP);
	}

	// 현재 씬 지정
	p_curScene = p_scenes[static_cast<unsigned int>(SCENE_TYPE::STAGE_01) - 1];
	p_curScene->Enter(nullptr);
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

void SceneManager::ChangeRealScene(SCENE_TYPE sceneType, Object* player)
{
	p_curScene->Exit(nullptr);

	g_prevStageNum = p_curScene->GetOwnStageNum();
	p_curScene = p_scenes[static_cast<UINT>(sceneType)];
	g_staticStage = p_curScene->GetOwnStageNum();

	p_curScene->Enter(player);
}


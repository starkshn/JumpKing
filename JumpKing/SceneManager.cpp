#include "pch.h"
#include "SceneManager.h"

#include "StartScene.h"
#include "StageScene.h"

#include "Object.h"

UINT SceneManager::g_nextStage = 0;
UINT SceneManager::g_staticStage = 0;
UINT SceneManager::g_prevStageNum = 0;

SceneManager::SceneManager()
	:
	_vecScenes(),
	// p_scenes{},
	p_curScene(nullptr)
{

}

SceneManager::~SceneManager()
{
	SafeDeleteVector(_vecScenes);

	int size = _vecScenes.size();

	int a = 10;
}

void SceneManager::Init()
{
	// Scene ����
	_vecScenes.reserve(static_cast<UINT>(SCENE_TYPE::END));

	for (UINT i = static_cast<unsigned int>(SCENE_TYPE::STAGE_1); i < static_cast<unsigned int>(SCENE_TYPE::STAGE_43); ++i)
	{
		_vecScenes.emplace_back(new StageScene(i));
	}

	// ���� �� ����
	p_curScene = *(_vecScenes.begin());
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

	g_prevStageNum = p_curScene->GetStageNumber();
	p_curScene = _vecScenes[static_cast<UINT>(sceneType)];
	g_staticStage = p_curScene->GetStageNumber();

	p_curScene->Enter(player);
}


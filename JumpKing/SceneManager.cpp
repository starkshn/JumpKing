#include "pch.h"
#include "SceneManager.h"

#include "StageScene.h"

#include "Object.h"

UINT SceneManager::g_nextStage = 0;
UINT SceneManager::g_staticStage = 0;

SceneManager::SceneManager()
	:
	_vecScenes(),
	p_curScene(nullptr)
{

}

SceneManager::~SceneManager()
{
	SafeDeleteVector(_vecScenes);
}

void SceneManager::Init()
{
	// Scene 생성
	_vecScenes.reserve(static_cast<UINT>(SCENE_TYPE::END));

	for (UINT i = 0; i < static_cast<unsigned int>(SCENE_TYPE::END); ++i)
	{
		_vecScenes.emplace_back(new StageScene(i));
	}

	// 현재 씬 지정
	p_curScene = *(_vecScenes.begin());

	_stageIter = _vecScenes.begin();

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

	p_curScene = _vecScenes[static_cast<UINT>(sceneType)];

	p_curScene->Enter(player);
}


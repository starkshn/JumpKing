#include "pch.h"
#include "StageScene.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "ColliderManager.h"
#include "CameraManager.h"

#include "Core.h"
#include "Texture.h"
#include "Object.h"
#include "Player.h"


using namespace std;

StageScene::StageScene()
	:
	p_backGroundTexture(nullptr),
	_stageNum()
{
	_stageNum = SceneManager::GetInstance()->GetStageNum();
	string str = to_string(_stageNum);
	wstring strP = wstring(str.begin(), str.end());

	p_backGroundTexture = ResourceManager::GetInstance()->LoadTexture(L"Stage_" + strP, L"Textures\\Stage\\Stage_" + strP + L".bmp");
}

StageScene::~StageScene()
{

}

void StageScene::Enter(Object* player)
{
	Vector2 resolution = Core::GetInstance()->GetResolution();

	player->SetPos(Vector2(resolution._x / 2.f, resolution._y / 2.f));
	player->SetObjectName(L"Player");
	player->SetPos(Vector2(640.f, 384.f));
	player->SetScale(Vector2(90.f, 103.f));
	AddObject(player, GROUP_TYPE::PLAYER);

	// 땅과 플레이어 충돌 지정
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// =================================

	// =================================
	// Camera Look 지정
	CameraManager::GetInstance()->SetLookAtPos(resolution / 2.f);
	// =================================


	// Init 함수 호출
	Init();
	// =======================================
}

void StageScene::Update()
{
	/*if (KEY_TAP(KEY::UP))
	{
		SceneManager::GetInstance()->UpStageNum();
		UINT sn = SceneManager::GetInstance()->GetStageNum();
		SCENE_TYPE s = static_cast<SCENE_TYPE>(sn);

		if (nullptr == SceneManager::GetInstance()->GetSceneArr(s))
		{
			StageScene* nextStage = new StageScene();

			SceneManager::GetInstance()->SetSceneArr(s, nextStage);
		}

		ChangeScene(s);
	}
	if (KEY_TAP(KEY::DOWN))
	{
		SceneManager::GetInstance()->DownStageNum();
		UINT sn = SceneManager::GetInstance()->GetStageNum();
		SCENE_TYPE s = static_cast<SCENE_TYPE>(sn);

		if (nullptr == SceneManager::GetInstance()->GetSceneArr(s))
		{
			StageScene* nextStage = new StageScene();

			SceneManager::GetInstance()->SetSceneArr(s, nextStage);
		}

		ChangeScene(s);
	}*/

	for (unsigned int i = 0; i < static_cast<unsigned int>(GROUP_TYPE::END); ++i)
	{
		const vector<Object*>& vecObj = GetGroupObjects((GROUP_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			vecObj[j]->Update();
		}
	}

	Scene::Update();
}

void StageScene::Render(HDC dc)
{
	//  Background rendering
	Vector2 resolution = Core::GetInstance()->GetResolution();
	BitBlt
	(
		dc,
		0, 0,
		(int)resolution._x,
		(int)resolution._y,
		p_backGroundTexture->GetDC(),
		0, 0, SRCCOPY
	);

	Scene::Render(dc);
}

void StageScene::Exit(Object* player)
{

}

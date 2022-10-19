#include "pch.h"
#include "StageScene.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "KeyManager.h"

#include "Core.h"
#include "Texture.h"


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

void StageScene::Enter()
{

}

void StageScene::Update()
{
	if (KEY_TAP(KEY::UP))
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
	}
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
}

void StageScene::Exit()
{

}

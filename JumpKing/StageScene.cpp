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
#include "Ground.h"

#include "RigidBody.h"
#include "resource.h"
#include "Scene.h"

StageScene::StageScene(UINT stageNumber)
	:
	p_backGroundTexture(nullptr),
	_colTypeInfo{}
{
	Core::GetInstance()->DockMenu();

	SetStageNumber(stageNumber);

	UINT sn = GetStageNumber();

	string str = to_string(sn);
	wstring strP = wstring(str.begin(), str.end());
	SetSceneName(L"Stage_" + strP);
	
	p_backGroundTexture = ResourceManager::GetInstance()->LoadTexture(GetSceneName(), L"Textures\\Stage\\Stage_" + strP + L".bmp");

	_changePos = CameraManager::GetInstance()->GetLookPos();
}

StageScene::~StageScene()
{

}

void StageScene::Enter(Object* player)
{
	// 종료할 때를 위한 g_staticScene 번호 저장
	UINT sn = GetStageNumber();
	SceneManager::GetInstance()->SetStaticStage(sn);

	_resolution = Core::GetInstance()->GetResolution();

	// Object 추가
	if (player == nullptr)
	{
		// Player 없을 경우
		Object* Pplayer = new Player();
		Pplayer->SetObjectName(L"Player");
		Pplayer->SetPos(Vector2(640.f, 384.f));
		Pplayer->SetScale(Vector2(90.f, 103.f));
		SceneManager::GetInstance()->AddObject(Pplayer, GROUP_TYPE::PLAYER);
		SetCurPlayer(Pplayer);
		RegisterPlayer(Pplayer);
	}
	else
	{
		player->SetPos(Vector2(player->GetPos()._x, player->GetPos()._y));
		RegisterPlayer(player);
	}

	// Side Ground 배치
	Object* leftSideGround = new Ground();
	leftSideGround->SetObjectName(L"Ground");
	leftSideGround->SetScale(Vector2(20.f, _resolution._y));
	leftSideGround->SetPos(Vector2(10.f, _resolution._y / 2.f));
	SceneManager::GetInstance()->AddObject(leftSideGround, GROUP_TYPE::GROUND);

	Object* rightSideGround = new Ground();
	rightSideGround->SetObjectName(L"Ground");
	rightSideGround->SetScale(Vector2(20.f, _resolution._y));
	rightSideGround->SetPos(Vector2(_resolution._x - 10.f, _resolution._y / 2.f));
	SceneManager::GetInstance()->AddObject(rightSideGround, GROUP_TYPE::GROUND);

	// Ground배치
	Object* ground = new Ground();
	ground->SetObjectName(L"Ground");
	ground->SetScale(Vector2(200.f, 100.f));
	ground->SetPos(Vector2(640.f, 700.f));
	SceneManager::GetInstance()->AddObject(ground, GROUP_TYPE::GROUND);

	Object* ground2 = new Ground();
	ground2->SetObjectName(L"Ground");
	ground2->SetScale(Vector2(200.f, 100.f));
	ground2->SetPos(Vector2(340.f, 300.f));
	SceneManager::GetInstance()->AddObject(ground2, GROUP_TYPE::GROUND);

	Object* ground3 = new Ground();
	ground3->SetObjectName(L"Ground");
	ground3->SetScale(Vector2(200.f, 100.f));
	ground3->SetPos(Vector2(940.f, 300.f));
	SceneManager::GetInstance()->AddObject(ground3, GROUP_TYPE::GROUND);

	// 땅과 플레이어 충돌 지정
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// =================================

	// =================================
	// Camera Look 지정
	CameraManager::GetInstance()->SetLookAtPos(_resolution / 2.f);
	// =================================

	// Init 함수 호출
	Init();
	// =======================================
}

void StageScene::Update()
{
	if (KEY_TAP(KEY::UP))
	{
		// 위로 올라감
		if (SceneManager::GetInstance()->UpStageByVec() != nullptr)
		{
			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());
		}

#pragma region "iter 안 쓴 버젼"
		//UINT curStageNumber = GetStageNumber();
		//UINT nextStageNumber = ++curStageNumber;

		//if (nextStageNumber >= static_cast<UINT>(SCENE_TYPE::END))
		//	return;

		//SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		//// Scene이 있는지 없는지 확인 하는 작업
		//if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		//{
		//	ChangeScene(nextST, GetCurPlayer());
		//}
#pragma endregion		
	}

	if (KEY_TAP(KEY::DOWN))
	{
		if (SceneManager::GetInstance()->DownStageByVec() != nullptr)
		{
			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());
		}
	}

	if (CheckGroundType())
	{
		// 클릭을 통해서 데이터를 받았을 경우
		if (_colTypeInfo._check && _colTypeInfo._type == COLLIDER_TYPE::NONE)
			return;

		if (_colTypeInfo._check && _colTypeInfo._type == COLLIDER_TYPE::SQUARE)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				_colliderPos._startPos = CameraManager::GetInstance()->GetRenderPos(MOUSE_POS);

				ground = new Ground();
				ground->SetObjectName(L"Ground");
				ground->SetScale(Vector2(1, 1));
				ground->SetPos(_colliderPos._startPos);
				
				SceneManager::GetInstance()->AddObject(ground, GROUP_TYPE::GROUND);
			}

			if (KEY_HOLD(KEY::LBTN))
			{
				Vector2 _curMousePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

				float xScale = abs(_colliderPos._startPos._x - _curMousePos._x);
				float yScale = abs(_colliderPos._startPos._y - _curMousePos._y);

				ground->SetScale(Vector2(xScale, yScale));
			}

			if (KEY_AWAY(KEY::LBTN))
			{
				_colliderPos._endPos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

				float xScale = abs(_colliderPos._startPos._x - _colliderPos._endPos._x);
				float yScale = abs(_colliderPos._startPos._y - _colliderPos._endPos._y);

				ground->SetScale(Vector2(xScale, yScale));
				ground->Init();
			}
		}

		if (_colTypeInfo._check && _colTypeInfo._type == COLLIDER_TYPE::TRIANGLE)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				_colliderPos._startPos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);
			}

			if (KEY_AWAY(KEY::LBTN))
			{
				_colliderPos._endPos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);
			}
		}

		if (_colTypeInfo._check && _colTypeInfo._type == COLLIDER_TYPE::LINE)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				_colliderPos._startPos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);
			}

			if (KEY_AWAY(KEY::LBTN))
			{
				_colliderPos._endPos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);
			}
		}

	}
	
	Vector2 playerRenderPos = CameraManager::GetInstance()->GetRenderPos(GetCurPlayer()->GetPos());

	if (playerRenderPos._y < 0.f)
	{
		// 위로 갈 때
  		ChangeStandPos(playerRenderPos, GetCurPlayer(), true);
	}

	if (playerRenderPos._y > _resolution._y)
	{
		// 아래로 갈 때
		ChangeStandPos(playerRenderPos, GetCurPlayer(), false);
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
	DeleteAllGroups();

	ColliderManager::GetInstance()->ResetGroup();

	SetCurPlayer(nullptr);
}

void StageScene::CreateForce()
{
	_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);
}

void StageScene::ChangeStandPos(Vector2 playerPos, Object* player, bool upDown)
{
	Vector2 resolution = Core::GetInstance()->GetResolution();

	if (upDown)
	{
		// 위로 올라감
		if (SceneManager::GetInstance()->UpStageByVec() != nullptr)
		{
			// 현재가 이전보다 크다 == 위로 올라감
			float curPosY = _resolution._y - (playerPos._y + player->GetScale()._y / 2.f);
			player->SetPos(Vector2(playerPos._x, curPosY));

			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());
		}
		
#pragma region "iter 안쓴 버전"
		//UINT curStageNumber = GetStageNumber();
		//UINT nextStageNumber = curStageNumber + 1;

		//if (curStageNumber + 1 == nextStageNumber)
		//{
		//	if (nextStageNumber >= static_cast<UINT>(SCENE_TYPE::END))
		//		return;

		//	SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		//	// Scene이 있는지 없는지 확인 하는 작업
		//	if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		//	{
		//		// 현재가 이전보다 크다 == 위로 올라감
		//		float curPosY = _resolution._y - (playerPos._y + player->GetScale()._y / 2.f);
		//		player->SetPos(Vector2(playerPos._x, curPosY));

		//		ChangeScene(nextST, GetCurPlayer());
		//	}
		//}
#pragma endregion
	}
	else
	{
		if (SceneManager::GetInstance()->DownStageByVec() != nullptr)
		{
			float curPosY = (playerPos._y + player->GetScale()._y / 2.f) - _resolution._y;
			player->SetPos(Vector2(playerPos._x, curPosY));

			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());
		}

#pragma region "iter 안 쓴 버젼"
		//UINT curStageNumber = GetStageNumber();
		//UINT nextStageNumber = curStageNumber - 1;

		//if (curStageNumber - 1 == nextStageNumber)
		//{
		//	if (nextStageNumber < static_cast<UINT>(SCENE_TYPE::STAGE_1))
		//		return;

		//	SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		//	// Scene이 있는지 없는지 확인 하는 작업
		//	if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		//	{
		//		// 현재가 이번보다 작다. == 밑으로 내려감
		//		float curPosY = (playerPos._y + player->GetScale()._y / 2.f) - _resolution._y;
		//		player->SetPos(Vector2(playerPos._x, curPosY));

		//		ChangeScene(nextST, player);
		//	}
		//}
#pragma endregion
	}
}


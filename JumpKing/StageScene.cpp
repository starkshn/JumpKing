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
	// ������ ���� ���� g_staticScene ��ȣ ����
	UINT sn = GetStageNumber();
	SceneManager::GetInstance()->SetStaticStage(sn);

	_resolution = Core::GetInstance()->GetResolution();

	// Object �߰�
	if (player == nullptr)
	{
		// Player ���� ���
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

	// Side Ground ��ġ
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

	// Ground��ġ
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

	// ���� �÷��̾� �浹 ����
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// =================================

	// =================================
	// Camera Look ����
	CameraManager::GetInstance()->SetLookAtPos(_resolution / 2.f);
	// =================================

	// Init �Լ� ȣ��
	Init();
	// =======================================
}

void StageScene::Update()
{
	if (KEY_TAP(KEY::UP))
	{
		// ���� �ö�
		if (SceneManager::GetInstance()->UpStageByVec() != nullptr)
		{
			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());
		}

#pragma region "iter �� �� ����"
		//UINT curStageNumber = GetStageNumber();
		//UINT nextStageNumber = ++curStageNumber;

		//if (nextStageNumber >= static_cast<UINT>(SCENE_TYPE::END))
		//	return;

		//SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		//// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
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
		// Ŭ���� ���ؼ� �����͸� �޾��� ���
		if (_colTypeInfo._check && _colTypeInfo._type == COLLIDER_TYPE::NONE)
			return;

		if (_colTypeInfo._check && _colTypeInfo._type == COLLIDER_TYPE::SQUARE)
		{
			if (KEY_TAP(KEY::LBTN))
			{
				_colliderPos._startPos = CameraManager::GetInstance()->GetRenderPos(MOUSE_POS);

				p_ground = new Ground();
				p_ground->SetObjectName(L"Ground");
				p_ground->SetScale(Vector2(1, 1));
				p_ground->SetPos(_colliderPos._startPos);
				
				SceneManager::GetInstance()->AddObject(p_ground, GROUP_TYPE::GROUND);
			}

			if (KEY_HOLD(KEY::LBTN))
			{
				Vector2 _curMousePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

				float xScale = abs(_colliderPos._startPos._x - _curMousePos._x);
				float yScale = abs(_colliderPos._startPos._y - _curMousePos._y);

				p_ground->SetScale(Vector2(xScale, yScale));
			}

			if (KEY_AWAY(KEY::LBTN))
			{
				_colliderPos._endPos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

				float xScale = abs(_colliderPos._startPos._x - _colliderPos._endPos._x);
				float yScale = abs(_colliderPos._startPos._y - _colliderPos._endPos._y);

				p_ground->SetScale(Vector2(xScale, yScale));
				p_ground->Init();
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

	if (KEY_TAP(KEY::RBTN))
	{
		p_targetGround = nullptr;

		_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

		vector<Object*> vecGrounds = SceneManager::GetInstance()->GetObjectsVec(static_cast<UINT>(GROUP_TYPE::GROUND));

		for (int i = 0; i < vecGrounds.size(); ++i)
		{
			Vector2 gPos = vecGrounds[i]->GetPos();
			Vector2 gScale = vecGrounds[i]->GetScale();

			float rightXRange = gPos._x + gScale._x / 2.f;
			float leftXRange = gPos._x - gScale._x / 2.f;
			float topYRange = gPos._y - gScale._y / 2.f;
			float bottomYRange = gPos._y + gScale._y / 2.f;

			if (leftXRange <= _mouseForcePos._x && _mouseForcePos._x <= rightXRange && topYRange <= _mouseForcePos._y && _mouseForcePos._y <= bottomYRange)
			{
				if (vecGrounds[i]->GetObjectName() == L"Ground")
					p_targetGround = vecGrounds[i];
			}
		}
	}

	if (KEY_HOLD(KEY::RBTN))
	{
		_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

		p_targetGround->SetPos(_mouseForcePos);
	}

	if (KEY_AWAY(KEY::RBTN))
	{
		_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

		p_targetGround->SetPos(_mouseForcePos);

		p_targetGround = nullptr;
	}
	
	Vector2 playerRenderPos = CameraManager::GetInstance()->GetRenderPos(GetCurPlayer()->GetPos());

	if (playerRenderPos._y < 0.f)
	{
		// ���� �� ��
  		ChangeStandPos(playerRenderPos, GetCurPlayer(), true);
	}

	if (playerRenderPos._y > _resolution._y)
	{
		// �Ʒ��� �� ��
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
		// ���� �ö�
		if (SceneManager::GetInstance()->UpStageByVec() != nullptr)
		{
			// ���簡 �������� ũ�� == ���� �ö�
			float curPosY = _resolution._y - (playerPos._y + player->GetScale()._y / 2.f);
			player->SetPos(Vector2(playerPos._x, curPosY));

			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());
		}
		
#pragma region "iter �Ⱦ� ����"
		//UINT curStageNumber = GetStageNumber();
		//UINT nextStageNumber = curStageNumber + 1;

		//if (curStageNumber + 1 == nextStageNumber)
		//{
		//	if (nextStageNumber >= static_cast<UINT>(SCENE_TYPE::END))
		//		return;

		//	SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		//	// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
		//	if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		//	{
		//		// ���簡 �������� ũ�� == ���� �ö�
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

#pragma region "iter �� �� ����"
		//UINT curStageNumber = GetStageNumber();
		//UINT nextStageNumber = curStageNumber - 1;

		//if (curStageNumber - 1 == nextStageNumber)
		//{
		//	if (nextStageNumber < static_cast<UINT>(SCENE_TYPE::STAGE_1))
		//		return;

		//	SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		//	// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
		//	if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		//	{
		//		// ���簡 �̹����� �۴�. == ������ ������
		//		float curPosY = (playerPos._y + player->GetScale()._y / 2.f) - _resolution._y;
		//		player->SetPos(Vector2(playerPos._x, curPosY));

		//		ChangeScene(nextST, player);
		//	}
		//}
#pragma endregion
	}
}


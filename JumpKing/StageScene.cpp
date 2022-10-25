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

StageScene::StageScene(UINT stageNumber)
	:
	p_backGroundTexture(nullptr)
{
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
	_resolution = Core::GetInstance()->GetResolution();
	UINT sn = GetStageNumber();
	SceneManager::GetInstance()->SetStaticStage(sn);

	// Object �߰�
	if (player == nullptr)
	{
		// Player ���� ���
		Object* Pplayer = new Player();
		Pplayer->SetObjectName(L"Player");
		Pplayer->SetPos(Vector2(640.f, 384.f));
		Pplayer->SetScale(Vector2(90.f, 103.f));
		AddObject(Pplayer, GROUP_TYPE::PLAYER);
		SetCurPlayer(Pplayer);
		RegisterPlayer(Pplayer);

		// Ground��ġ
		Object* ground = new Ground();
		ground->SetObjectName(L"Ground");
		ground->SetScale(Vector2(400.f, 100.f));
		ground->SetPos(Vector2(640.f, 700.f));
		AddObject(ground, GROUP_TYPE::GROUND);

		Object* ground2 = new Ground();
		ground2->SetObjectName(L"Ground");
		ground2->SetScale(Vector2(400.f, 100.f));
		ground2->SetPos(Vector2(640.f, 500.f));
		AddObject(ground2, GROUP_TYPE::GROUND);

		Object* ground3 = new Ground();
		ground3->SetObjectName(L"Ground");
		ground3->SetScale(Vector2(200.f, 100.f));
		ground3->SetPos(Vector2(340.f, 200.f));
		AddObject(ground3, GROUP_TYPE::GROUND);

		Object* ground4 = new Ground();
		ground4->SetObjectName(L"Ground");
		ground4->SetScale(Vector2(200.f, 100.f));
		ground4->SetPos(Vector2(940.f, 200.f));
		AddObject(ground4, GROUP_TYPE::GROUND);
	}
	else
	{		
		player->SetPos(Vector2(player->GetPos()._x, player->GetPos()._y));
		AddObject(player, GROUP_TYPE::PLAYER);
		RegisterPlayer(player);
	}

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
	if (KEY_HOLD(KEY::LBTN))
	{
		_useForce = true;

		CreateForce();
	}
	else
	{
		_useForce = false;
	}

	for (unsigned int i = 0; i < static_cast<unsigned int>(GROUP_TYPE::END); ++i)
	{
		const vector<Object*>& vecObj = GetGroupObjects((GROUP_TYPE)i);

		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (_useForce && nullptr != vecObj[j]->GetRigidBody())
			{
				Vector2 diff = vecObj[j]->GetPos() - _mouseForcePos;
				float len = diff.Length();
				Vector2 tempDiff = diff;

				if (len < _forceRadius)
				{
					float ratio = 1.f - (len / _forceRadius);
					float force = _force * ratio;

					vecObj[j]->GetRigidBody()->AddForce(tempDiff.Normalize() * force);
				}
			}

			vecObj[j]->Update();
		}
	}

	if (KEY_TAP(KEY::UP))
	{
		// ���� �ö�
		UINT curStageNumber = GetStageNumber();
		UINT nextStageNumber = ++curStageNumber;

		if (nextStageNumber >= static_cast<UINT>(SCENE_TYPE::END))
			return;

		SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
		if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		{
			ChangeScene(nextST, GetCurPlayer());
		}
	}

	if (KEY_TAP(KEY::DOWN))
	{
		// ���� �ö�
		UINT curStageNumber = GetStageNumber();
		UINT nextStageNumber = --curStageNumber;

		if (nextStageNumber >= static_cast<UINT>(SCENE_TYPE::END))
			return;

		SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
		if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		{
			ChangeScene(nextST, GetCurPlayer());
		}
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
		UINT curStageNumber = GetStageNumber();
		UINT nextStageNumber = curStageNumber + 1;

		if (curStageNumber + 1 == nextStageNumber)
		{
			if (nextStageNumber >= static_cast<UINT>(SCENE_TYPE::END))
				return;

			SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

			// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
			if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
			{
				// ���簡 �������� ũ�� == ���� �ö�
				float curPosY = _resolution._y - (playerPos._y + player->GetScale()._y / 2.f);
				player->SetPos(Vector2(playerPos._x, curPosY));

				ChangeScene(nextST, GetCurPlayer());
			}
		}
	}
	else
	{
		UINT curStageNumber = GetStageNumber();
		UINT nextStageNumber = curStageNumber - 1;

		if (curStageNumber - 1 == nextStageNumber)
		{
			if (nextStageNumber < static_cast<UINT>(SCENE_TYPE::STAGE_1))
				return;

			SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

			// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
			if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
			{
				// ���簡 �̹����� �۴�. == ������ ������
				float curPosY = (playerPos._y + player->GetScale()._y / 2.f) - _resolution._y;
				player->SetPos(Vector2(playerPos._x, curPosY));
				ChangeScene(nextST, player);
			}
		}
	}
}

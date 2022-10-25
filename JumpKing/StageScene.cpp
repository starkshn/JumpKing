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
	
	p_backGroundTexture = ResourceManager::GetInstance()->LoadTexture(GetSceneName(), L"Textures\\Stage\\" + strP + L".bmp");

	_changePos = CameraManager::GetInstance()->GetLookPos();
}

StageScene::~StageScene()
{

}

void StageScene::Enter(Object* player)
{
	_resolution = Core::GetInstance()->GetResolution();

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
		UINT curStageNumber = GetStageNumber();
		UINT prevStageNumber = SceneManager::GetInstance()->GetPrevStage();

		Vector2 prevPos;
		float curPosY = 0;
		
		// Player ���� ���
		if (curStageNumber > prevStageNumber)
		{
			// ���簡 �������� ũ�� == ���� �ö�
			prevPos = player->GetPos();
			curPosY = _resolution._y - (prevPos._y + player->GetScale()._y / 2.f);
		}

		if (curStageNumber < prevStageNumber)
		{
			// ���簡 �̹����� �۴�. == ������ ������
			prevPos = player->GetPos();
			curPosY = _resolution._y + (prevPos._y + player->GetScale()._y / 2.f);
		}
		
		player->SetPos(Vector2(prevPos._x, curPosY));
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

	if (GetCurPlayer()->GetPos()._y < 0.f)
	{
		ChangeStandPos(GetCurPlayer()->GetPos(), GetCurPlayer(), true);
	}

	if (GetCurPlayer()->GetPos()._y > _resolution._y)
	{
		ChangeStandPos(GetCurPlayer()->GetPos(), GetCurPlayer(), false);
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
	Vector2 playerRenderPos = CameraManager::GetInstance()->GetRenderPos(playerPos);

	if (upDown)
	{
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
	else
	{
		UINT curStageNumber = GetStageNumber();
		UINT nextStageNumber = --curStageNumber;

		if (nextStageNumber < static_cast<UINT>(SCENE_TYPE::STAGE_1))
			return;

		SCENE_TYPE nextST = static_cast<SCENE_TYPE>(nextStageNumber);

		// Scene�� �ִ��� ������ Ȯ�� �ϴ� �۾�
		if (SceneManager::GetInstance()->GetScene(nextST) != nullptr)
		{
			ChangeScene(nextST, player);
		}
	}
}

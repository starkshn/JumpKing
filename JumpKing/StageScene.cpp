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

StageScene::StageScene()
	:
	p_backGroundTexture(nullptr)
{
	string str = to_string(GetStageNum());
	wstring strP = wstring(str.begin(), str.end());

	p_backGroundTexture = ResourceManager::GetInstance()->LoadTexture(L"Stage_" + strP, L"Textures\\Stage\\Stage_" + strP + L".bmp");

	_changePos = CameraManager::GetInstance()->GetLookPos();
}

StageScene::~StageScene()
{

}

void StageScene::Enter(Object* player)
{
	_resolution = Core::GetInstance()->GetResolution();

	// Object 추가
	if (player == nullptr)
	{
		// Player 없을 경우
		Object* Pplayer = new Player();
		Pplayer->SetObjectName(L"Player");
		Pplayer->SetPos(Vector2(640.f, 384.f));
		Pplayer->SetScale(Vector2(90.f, 103.f));
		AddObject(Pplayer, GROUP_TYPE::PLAYER);
		SetCurPlayer(Pplayer);
		RegisterPlayer(Pplayer);

		// Ground배치
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
		UINT cur = GetOwnStageNum();
		UINT prev = SceneManager::GetInstance()->GetPrevStage();

		Vector2 prevPos;
		float curPosY = 0;
		
		// Player 있을 경우
		if (cur > prev)
		{
			// 현재가 이전보다 크다 == 위로 올라감
			prevPos = player->GetPos();
			curPosY = _resolution._y - (prevPos._y + player->GetScale()._y / 2.f);

			int a = 10;
		}

		if (cur < prev)
		{
			// 현재가 이번보다 작다. == 밑으로 내려감
			prevPos = player->GetPos();
			curPosY = _resolution._y + (prevPos._y + player->GetScale()._y / 2.f);

		}
		
		player->SetPos(Vector2(prevPos._x, curPosY));
		AddObject(player, GROUP_TYPE::PLAYER);
		RegisterPlayer(player);
	}

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

	ChangeStandPos(GetCurPlayer()->GetPos(), GetCurPlayer());

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

void StageScene::ChangeStandPos(Vector2 playerPos, Object* player)
{
	Vector2 resolution = Core::GetInstance()->GetResolution();
	Vector2 playerRenderPos = CameraManager::GetInstance()->GetRenderPos(playerPos);
	
	if (playerRenderPos._y < 0.f)
	{
		// 위로 올라감
		SceneManager::GetInstance()->UpStageNum();
		UINT sn = SceneManager::GetInstance()->GetStageNum();

		if (sn > static_cast<UINT>(SCENE_TYPE::STAGE_43))
			return;

		SCENE_TYPE s = static_cast<SCENE_TYPE>(sn);

		Scene* nextStage = nullptr;

		nextStage = SceneManager::GetInstance()->GetSceneArr(s, nextStage);

		if (nextStage == nullptr)
			assert(nullptr);
		else
			ChangeScene(s, GetCurPlayer());
	}

	if (playerRenderPos._y > resolution._y)
	{
		// 밑으로 내려감

		SceneManager::GetInstance()->DownStageNum();
		UINT sn = SceneManager::GetInstance()->GetStageNum();

		if (sn < static_cast<UINT>(SCENE_TYPE::STAGE_01))
			return;

		SCENE_TYPE s = static_cast<SCENE_TYPE>(sn);

		Scene* nextStage = nullptr;

		nextStage = SceneManager::GetInstance()->GetSceneArr(s, nextStage);

		if (nextStage == nullptr)
			assert(nullptr);
		else
			ChangeScene(s, player);
	}
}

#include "pch.h"
#include "StartScene.h"

#include "PathManager.h"
#include "ResourceManager.h"
#include "ColliderManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "Core.h"
#include "Object.h"
#include "Player.h"
#include "Texture.h"

// 물리
#include "RigidBody.h"
#include "GDI.h"

// 땅
#include "Ground.h"

StartScene::StartScene()
	:
	_useForce(false),
	_forceRadius(500.f),
	_forceCurRadius(0.f),
	_force(500.f)
{
	p_backGroundTexture = ResourceManager::GetInstance()->LoadTexture(L"Stage_1", L"Textures\\Stage\\Stage_1.bmp");
}

StartScene::~StartScene()
{
	// 씬 전부 삭제

}

void StartScene::Update()
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

	Scene::Update();

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeScene(SCENE_TYPE::TOOL);
	}
}

void StartScene::Enter()
{
	// Object 추가
	Object* player = new Player();
	player->SetObjectName(L"Player");
	player->SetPos(Vector2(640.f, 384.f));
	player->SetScale(Vector2(100.f, 100.f));

	AddObject(player, GROUP_TYPE::PLAYER);

	RegisterPlayer(player);

	Vector2 resolution = Core::GetInstance()->GetResolution();

	// ================================
	// 충돌 지정
	// Player 그룹과 Monster그룹 간의 충돌체크 ( Plyaer그룹과 Monster그룹이 충돌할 것이라고 알린다)
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);

	// 땅과 플레이어 충돌 지정
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// =================================


	// =================================
	// Camera Look 지정
	CameraManager::GetInstance()->SetLookAtPos(resolution / 2.f);
	// =================================

	// =================================
	// Camera Effect 지정
	CameraManager::GetInstance()->FadeOut(1.5f);
	CameraManager::GetInstance()->FadeIn(1.5f);
	// =================================

	// =======================================
	// Init 함수 호출
	Init();
	// =======================================

}

void StartScene::Render(HDC dc)
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

	// 힘이 생기는 지점 원 그리기
	if (_useForce)
	{
		GDI gdi(dc, HBRUSH_TYPE::HOLLOW);
		GDI gdi2(dc, HPEN_TYPE::GREEN);

		_forceCurRadius += _forceRadius * 5.f * DeltaTime_F;
		if (_forceCurRadius > _forceRadius)
		{
			_forceCurRadius = 0.f;
		}

		Vector2 _renderPos = CameraManager::GetInstance()->GetRenderPos(_mouseForcePos);

		Ellipse
		(
			dc,
			static_cast<int>(_renderPos._x - _forceCurRadius),
			static_cast<int>(_renderPos._y - _forceCurRadius),
			static_cast<int>(_renderPos._x + _forceCurRadius),
			static_cast<int>(_renderPos._y + _forceCurRadius)
		);
	}

	Scene::Render(dc);
}

void StartScene::Exit()
{
	DeleteAllGroups();

	// 기존의 그룹의 충돌 그룹 해제(씬이 변경될 것이니까)
	ColliderManager::GetInstance()->ResetGroup();
}

void StartScene::CreateForce()
{
	_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);
}



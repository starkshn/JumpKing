#include "pch.h"
#include "StartScene.h"

#include "PathManager.h"
#include "ResourceManager.h"
#include "ColliderManager.h"
#include "KeyManager.h"
#include "SceneManager.h"
#include "CameraManager.h"
#include "TimeManager.h"

#include "Core.h"
#include "Object.h"
#include "Player.h"
#include "Texture.h"
#include "StageScene.h"

// ����
#include "RigidBody.h"
#include "GDI.h"

// ��
#include "Ground.h"

StartScene::StartScene()
	:
	_useForce(false),
	_forceRadius(500.f),
	_forceCurRadius(0.f),
	_force(500.f)
{
	p_backGroundTexture = ResourceManager::GetInstance()->LoadTexture(L"Stage_1", L"Textures\\Stage\\Stage_1.bmp");

	_resolution = Core::GetInstance()->GetResolution();
}
 
StartScene::~StartScene()
{

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

	if (KEY_TAP(KEY::UP))
	{
		/*SceneManager::GetInstance()->UpStageNum();
		UINT sn = SceneManager::GetInstance()->GetStageNum();
		SCENE_TYPE s = static_cast<SCENE_TYPE>(sn);

		StageScene* nextStage = new StageScene();

		SceneManager::GetInstance()->SetSceneArr(s, nextStage);

		ChangeScene(s);*/
	}

	if (GetCurPlayer()->GetPos()._y < 0)
	{
		SceneManager::GetInstance()->UpStageNum();
		UINT sn = SceneManager::GetInstance()->GetStageNum();
		SCENE_TYPE s = static_cast<SCENE_TYPE>(sn);

		Scene* nextStage = nullptr;

		Object* playerPtr = GetCurPlayer();
		
		nextStage = SceneManager::GetInstance()->GetSceneArr(s, nextStage);

		if (nextStage == nullptr)
			assert(nullptr);
		else
			ChangeScene(s, playerPtr);
	}
}

void StartScene::Enter(Object* player)
{
	Core::GetInstance()->DockMenu();

	// Object �߰�
	Object* Pplayer = new Player();
	Pplayer->SetObjectName(L"Player");
	Pplayer->SetPos(Vector2(640.f, 384.f));
	Pplayer->SetScale(Vector2(90.f, 103.f));
	AddObject(Pplayer, GROUP_TYPE::PLAYER);
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

	Vector2 resolution = Core::GetInstance()->GetResolution();

	// ================================
	// �浹 ����
	//
	// ���� �÷��̾� �浹 ����
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// =================================


	// =================================
	// Camera Look ����
	CameraManager::GetInstance()->SetLookAtPos(resolution / 2.f);
	// =================================

	// =================================
	// Camera Effect ����
	CameraManager::GetInstance()->FadeOut(1.5f);
	CameraManager::GetInstance()->FadeIn(1.5f);
	// =================================

	// =======================================
	// Init �Լ� ȣ��
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

	// ���� ����� ���� �� �׸���
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

void StartScene::Exit(Object* player)
{
	//DeleteAllGroups();

	//// ������ �׷��� �浹 �׷� ����(���� ����� ���̴ϱ�)
	//ColliderManager::GetInstance()->ResetGroup();
}

void StartScene::CreateForce()
{
	_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);
}



#include "pch.h"
#include "StageScene.h"

#include "SceneManager.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "KeyManager.h"
#include "ColliderManager.h"
#include "CameraManager.h"
#include "PathManager.h"

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
	// [Menu Bar Setting]
	Core::GetInstance()->DockMenu();

	// [StageSceneNumber Setting]
	SetStageNumber(stageNumber);
	UINT sn = GetStageNumber();

	_str = to_string(sn);
	_strP = wstring(_str.begin(), _str.end());
	SetSceneName(L"Stage_" + _strP);

	// [Load Texture]
	p_backGroundTexture = ResourceManager::GetInstance()->LoadTexture(GetSceneName(), L"Textures\\Stage\\Stage_" + _strP + L".bmp");
	_changePos = CameraManager::GetInstance()->GetLookPos();

	// [Load Ground Infomation]
	if (sn < 4)
	{
		wstring path = L"Stage\\Stage_" + _strP + L".stage";
		LoadGroundInfo(path);
	}
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

	for (int i = 0; i < _vecGroundInfo.size(); ++i)
	{
		Object* ground = new Ground();
		ground->SetObjectName(L"Ground");
		ground->SetScale(_vecGroundInfo[i]._scale);
		ground->SetPos(_vecGroundInfo[i]._pos);
		SceneManager::GetInstance()->AddObject(ground, GROUP_TYPE::GROUND);
	}
	// ���� �÷��̾� �浹 ����
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// =================================

	// =================================
	// Camera Look ����
	CameraManager::GetInstance()->SetLookAtPos(_resolution / 2.f);
	// =================================

	// =================================
	// Init �Լ� ȣ��
	Init();
	// =================================
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

	// ������ �׶��� ����
	if (KEY_TAP(KEY::Z))
	{
		if (p_targetGround == nullptr)
			return;
		DeleteObjectEvent(p_targetGround);
	}

	// Ground Ŭ���� �����̴� �κ�
	if (KEY_TAP(KEY::RBTN))
	{
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

	// Ground Ŭ���� �����̴� �κ�
	if (KEY_HOLD(KEY::RBTN))
	{
		_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

		p_targetGround->SetPos(_mouseForcePos);
	}

	// Ground Ŭ���� �����̴� �κ�
	if (KEY_AWAY(KEY::RBTN))
	{
		_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

		p_targetGround->SetPos(_mouseForcePos);
	}
	
	Vector2 playerRenderPos = CameraManager::GetInstance()->GetRenderPos(GetCurPlayer()->GetPos());

	if (playerRenderPos._y - (GetCurPlayer()->GetScale()._y / 4.f) < 0.f)
	{
		// ���� �� ��
  		ChangeStandPos(playerRenderPos, GetCurPlayer(), true);
	}

	if (playerRenderPos._y + (GetCurPlayer()->GetScale()._y / 4.f) > _resolution._y)
	{
		// �Ʒ��� �� ��
		ChangeStandPos(playerRenderPos, GetCurPlayer(), false);
	}

	// StageScene Ground Information Save
	if (KEY_TAP(KEY::CTRL))
	{
		string p = "Stage\\Stage_" + _str + ".stage";
		wstring path = wstring(p.begin(), p.end());
		SaveGroundInfo(path);
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
			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());

			// ���簡 �������� ũ�� == ���� �ö�
			float curPosY = _resolution._y - (player->GetScale()._y / 4.f);
			player->SetPos(Vector2(playerPos._x, curPosY));
		}
	}
	else
	{
		// ������ ������
		if (SceneManager::GetInstance()->DownStageByVec() != nullptr)
		{
			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());

			float curPosY = (player->GetScale()._y / 4.f);
			player->SetPos(Vector2(playerPos._x, curPosY));
		}
	}
}

void StageScene::SaveGroundInfo(const wstring& relativePath)
{
	wstring filePath = PathManager::GetInstance()->GetAbsolutePath();
	filePath += relativePath; // contents ��� + ����� => ���� ���

	vector<Object*> vecGrounds = SceneManager::GetInstance()->GetObjectsVec(static_cast<UINT>(GROUP_TYPE::GROUND));

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");
	assert(file);

	fprintf(file, "[Ground Count](UINT)\n");
	fprintf(file, "%d\n", static_cast<UINT>(vecGrounds.size()));

	for (int i = 0; i < vecGrounds.size(); ++i)
	{
		fprintf(file, "[Ground Pos](Vector2)\n");
		fprintf(file, "%d %d\n", static_cast<int>(vecGrounds[i]->GetPos()._x), static_cast<int>(vecGrounds[i]->GetPos()._y));
		fprintf(file, "[Ground Scale](Vector2)\n");
		fprintf(file, "%d %d\n", static_cast<int>(vecGrounds[i]->GetScale()._x), static_cast<int>(vecGrounds[i]->GetScale()._y));
	}

	fclose(file);
}

void StageScene::LoadGroundInfo(const wstring& relativePath)
{
	wstring filePath = PathManager::GetInstance()->GetAbsolutePath();
	filePath += relativePath; // contents ��� + ����� => ���� ���

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");
	assert(file);

	char buffer[256] = {};

	string str;
	FScanf(buffer, file); // [Ground Count](UINT)
	FScanf(buffer, file); // ����
	str = buffer;

	UINT gc = std::stoi(str);
	for (int i = 0; i < gc; ++i)
	{
		POINT pt = {};
		GROUND_INFO gi = {};

		if (i >= 1)
			FScanf(buffer, file); // '\n' �о����.

		FScanf(buffer, file); // [Ground Pos]
		if (!strcmp("[Ground Pos](Vector2)", buffer)) // ������ 0�� ��ȯ
		{
			fscanf_s(file, "%d", &pt.x);
			fscanf_s(file, "%d", &pt.y);

			gi._pos = Vector2(pt);
		}

		FScanf(buffer, file); // '\n' �������ʴ� '\n' �о����.
		FScanf(buffer, file); // [Ground Scale]

		if (!strcmp("[Ground Scale](Vector2)", buffer)) // ������ 0�� ��ȯ
		{
			fscanf_s(file, "%d", &pt.x);
			fscanf_s(file, "%d", &pt.y);

			gi._scale = Vector2(pt);
		}

		_vecGroundInfo.push_back(gi);
	}

	fclose(file);
}

void StageScene::DeleteGround()
{
	
}


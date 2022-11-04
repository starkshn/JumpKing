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

	for (int i = 0; i < _vecGroundInfo.size(); ++i)
	{
		Object* ground = new Ground();
		ground->SetObjectName(L"Ground");
		ground->SetScale(_vecGroundInfo[i]._scale);
		ground->SetPos(_vecGroundInfo[i]._pos);
		SceneManager::GetInstance()->AddObject(ground, GROUP_TYPE::GROUND);
	}
	// 땅과 플레이어 충돌 지정
	ColliderManager::GetInstance()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::GROUND);
	// =================================

	// =================================
	// Camera Look 지정
	CameraManager::GetInstance()->SetLookAtPos(_resolution / 2.f);
	// =================================

	// =================================
	// Init 함수 호출
	Init();
	// =================================
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

	// 생성한 그라운드 삭제
	if (KEY_TAP(KEY::Z))
	{
		if (p_targetGround == nullptr)
			return;
		DeleteObjectEvent(p_targetGround);
	}

	// Ground 클릭후 움직이는 부분
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

	// Ground 클릭후 움직이는 부분
	if (KEY_HOLD(KEY::RBTN))
	{
		_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

		p_targetGround->SetPos(_mouseForcePos);
	}

	// Ground 클릭후 움직이는 부분
	if (KEY_AWAY(KEY::RBTN))
	{
		_mouseForcePos = CameraManager::GetInstance()->GetRealPos(MOUSE_POS);

		p_targetGround->SetPos(_mouseForcePos);
	}
	
	Vector2 playerRenderPos = CameraManager::GetInstance()->GetRenderPos(GetCurPlayer()->GetPos());

	if (playerRenderPos._y - (GetCurPlayer()->GetScale()._y / 4.f) < 0.f)
	{
		// 위로 갈 때
  		ChangeStandPos(playerRenderPos, GetCurPlayer(), true);
	}

	if (playerRenderPos._y + (GetCurPlayer()->GetScale()._y / 4.f) > _resolution._y)
	{
		// 아래로 갈 때
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
		// 위로 올라감
		if (SceneManager::GetInstance()->UpStageByVec() != nullptr)
		{
			ChangeScene(static_cast<SCENE_TYPE>((SceneManager::GetInstance()->GetSceneByVec())->GetStageNumber()), GetCurPlayer());

			// 현재가 이전보다 크다 == 위로 올라감
			float curPosY = _resolution._y - (player->GetScale()._y / 4.f);
			player->SetPos(Vector2(playerPos._x, curPosY));
		}
	}
	else
	{
		// 밑으로 내려감
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
	filePath += relativePath; // contents 경로 + 상대경로 => 최종 경로

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
	filePath += relativePath; // contents 경로 + 상대경로 => 최종 경로

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");
	assert(file);

	char buffer[256] = {};

	string str;
	FScanf(buffer, file); // [Ground Count](UINT)
	FScanf(buffer, file); // 갯수
	str = buffer;

	UINT gc = std::stoi(str);
	for (int i = 0; i < gc; ++i)
	{
		POINT pt = {};
		GROUND_INFO gi = {};

		if (i >= 1)
			FScanf(buffer, file); // '\n' 읽어들임.

		FScanf(buffer, file); // [Ground Pos]
		if (!strcmp("[Ground Pos](Vector2)", buffer)) // 같으면 0을 반환
		{
			fscanf_s(file, "%d", &pt.x);
			fscanf_s(file, "%d", &pt.y);

			gi._pos = Vector2(pt);
		}

		FScanf(buffer, file); // '\n' 보이지않는 '\n' 읽어들임.
		FScanf(buffer, file); // [Ground Scale]

		if (!strcmp("[Ground Scale](Vector2)", buffer)) // 같으면 0을 반환
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


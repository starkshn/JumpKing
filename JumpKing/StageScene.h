#pragma once
#include "Scene.h"

struct COLLIDER_POS
{
	Vector2 _startPos;
	Vector2 _endPos;
};

struct GROUND_INFO
{
	Vector2 _pos;
	Vector2 _scale;
};

class StageScene : public Scene
{
private:
	Texture*	p_backGroundTexture;

	StageScene* p_vecStages[static_cast<UINT>(SCENE_TYPE::END)];
	vector<GROUND_INFO> _vecGroundInfo = {};

	Vector2		_mouseForcePos;
	Vector2		_resolution;
	Vector2		_changePos;

	bool		_useForce;
	bool		_squarG;
	bool		_squarT;
	bool		_checkGroundType;

	float		_forceCurRadius;
	float		_forceRadius;
	float		_force;

	wstring		_strP;
	string		_str;

	// 메뉴바에서 입력받은 타입
	COLLIDER_TYPE_INFO _colTypeInfo;

	// Collider 그릴 위치
	COLLIDER_POS _colliderPos;

	// Ground
	Object* p_ground = nullptr;
	Object* p_targetGround = nullptr;

public:
	StageScene(UINT stageNumber);
	virtual ~StageScene() override;

private:
	virtual void Enter(Object* player) override;
	virtual void Exit(Object* player) override;
	virtual void Update() override;
	virtual void Render(HDC dc) override;

public:
	void ChangeStandPos(Vector2 playerPos, Object* player, bool upDown);
	
public:
	// JumpKing.cpp 에서 사각형인지를 셋팅받는 함수
	void SetGroundTypeInfo(const COLLIDER_TYPE_INFO& s) { _colTypeInfo = s; _checkGroundType = true; }

public:
	const COLLIDER_TYPE_INFO& GetGroundTypeInfo() { return _colTypeInfo ; }
	bool CheckGroundType() { return _checkGroundType; }
	
public:
	void CreateForce();

public:
	void SaveGroundInfo(const wstring& relativePath);
	void LoadGroundInfo(const wstring& relativePath);
};


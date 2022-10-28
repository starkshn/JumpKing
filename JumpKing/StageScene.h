#pragma once
#include "Scene.h"

class StageScene : public Scene
{
private:
	Texture*	p_backGroundTexture;

	StageScene* p_vecStages[static_cast<UINT>(SCENE_TYPE::END)];

	Vector2		_mouseForcePos;
	Vector2		_resolution;
	Vector2		_changePos;

	bool		_useForce;
	bool		_squarG;
	bool		_squarT;

	float		_forceCurRadius;
	float		_forceRadius;
	float		_force;

	COLLIDER_TYPE_INFO _colTypeInfo;


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
	void SetGroundTypeInfo(const COLLIDER_TYPE_INFO& s) { _colTypeInfo = s; }

public:
	const COLLIDER_TYPE_INFO& GetGroundTypeInfo() { return _colTypeInfo ; }
	
public:
	void CreateForce();

};


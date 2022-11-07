#pragma once
#include "Object.h"

class Texture;
class Sound;

class Player : public Object
{
private:
	CLONE(Player);

	OBJECT_STATE _curState;
	OBJECT_STATE _prevState;

	// 충돌시 어느면에 충돌했는지 확인하기 위한 구조체
	PLAYER_COL_INFO _playerColInfo;

	int _dir;
	int _prevDir;

	bool _onJump;

	Vector2 _fallDir = { 0.f, 1.f };

	Sound* p_jump;
	Sound* p_land;
	Sound* p_bump;
	Sound* p_fall;

public:
	Player();
	virtual ~Player() override;
	
public:
	virtual void Update() final;
	virtual void Render(HDC dc) final;

public:
	void UpdateState();
	void UpdateMove();
	void UpdateAnimation();
	void UpdateGravity();

public:
	virtual void OnCollisionEnter(Collider* other);
	virtual void OnCollisionStay(Collider* other);
	virtual void OnCollisionExit(Collider* other);

public:
	void SetPlayerColInfo(PLAYER_COL_INFO info) { _playerColInfo = info; }
	void SetState(const OBJECT_STATE& state) { _curState = state; }

public:
	OBJECT_STATE GetCurState() { return _curState; }
	const PLAYER_COL_INFO& GetPlayerColInfo() { return _playerColInfo; }
	int GetPlayerDir() { return _dir; }
};


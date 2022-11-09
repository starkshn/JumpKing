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

	UINT _jumpLevel;
	float _acc = 0.f;

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

	UINT GetJumpLevel(float& acc)
	{
		UINT jumpLevel = 0;

		if (acc >= 2.f)
		{
			jumpLevel = 1;
			acc = 0.f;
			return jumpLevel;
		}
		else if (acc >= 1.f)
		{
			jumpLevel = 2;
			acc = 0.f;
			return jumpLevel;
		}
		else
		{
			jumpLevel = 3;
			acc = 0.f;
			return jumpLevel;
		}
	}

public:
	bool CheckColDir(Object* otherObj);

	Vector2 VelocityToPercent(const Vector2& velocity)
	{
		Vector2 originVelocity = { 400.f, -2000.f };

		float xPercent = (velocity._x / originVelocity._x) * 100;
		float yPercent = (velocity._y / originVelocity._y) * 100;

		return Vector2(xPercent, yPercent);
	}
};


#pragma once
#include "Object.h"

class Texture;

class Player : public Object
{
private:
	CLONE(Player);

	OBJECT_STATE _curState;
	OBJECT_STATE _prevState;

	int _dir;
	int _prevDir;
	bool _onJump;

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

};


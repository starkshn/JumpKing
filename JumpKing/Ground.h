#pragma once
#include "Object.h"
class Ground : public Object
{
private:
	CLONE(Ground);

	Vector2 _pos;
	Vector2 _scale;

	bool _onGround;

public:
	Ground();
	virtual ~Ground() override;

public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(HDC dc) override;

public:
	virtual void OnCollisionEnter(Collider* other);
	virtual void OnCollisionStay(Collider* other);
	virtual void OnCollisionExit(Collider* other);

public:
	// virtual void SetPos(Vector2 pos) override { _pos = pos; }

	void SetOnGround(bool onGround) { _onGround = onGround; }

public:
	void CheckColDir(Object* otherObj);
};


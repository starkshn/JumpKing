#pragma once

class Object;

class Collider
{
private:
	static UINT g_ID;

	UINT		_ID = 0;
	UINT		_colCount;

	Object*		p_owner;

	Vector2		_offsetPos;
	Vector2		_finalPos;
	Vector2		_colliderScale;

	bool		_active;
	

	COLLIDER_TYPE_INFO _colTypeInfo;

public:
	Collider();
	Collider(const Collider& origin);
	~Collider();

public:
	void FinalUpdate();
	void Render(HDC dc);

public:
	void OnCollisonStay(Collider* other);
	void OnCollisionEnter(Collider* other);
	void OnCollisionExit(Collider* other);

	Collider& operator = (const Collider& other) = delete;

public:
	void SetOffsetPos(Vector2 pos) { _offsetPos = pos; }
	void SetColliderScale(Vector2 scale) { _colliderScale = scale; }

public:
	Vector2	GetOffsetPos() { return _offsetPos; }
	Vector2	GetColliderScale() { return _colliderScale; }
	Vector2	GetFinalPos() { return _finalPos; }
	UINT	GetID() { return _ID; }
	Object* GetColliderOwner() { return p_owner; }

	friend class Object;
};


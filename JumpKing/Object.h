#pragma once

#include "CameraManager.h"
#include "ResourceManager.h"

class Components;
class Collider;
class Animator;
class RigidBody;
class Texture;
class Gravity;


class Object
{
private:
	wstring _objectName;
	bool	_alive;

	float	_theta;

	bool	_onCollision;
	bool	_onLeftRight;
	
	Vector2 _dir;
	Vector2 _pos;
	Vector2 _scale;

	// Components
	Collider* p_collider;
	Animator* p_animator;
	RigidBody* p_rigidBody;
	Gravity* p_gravity;

public:
	Object();
	Object(const Object& origin);
	virtual ~Object();

	virtual Object* Clone() abstract;

public:
	virtual void Init() {}; // Scene이 시작되기 직전에 호출되는 함수
	virtual void Update() abstract;
	virtual void FinalUpdate();
	virtual void Render(HDC dc);

	void ComponentRender(HDC dc);

public:
	void SetDead() { _alive = false; }
	bool IsDead() { return !_alive; }

public:
	void CreateCollider();
	void CreateAnimator();
	void CreateAnimation(const wstring& animName, Texture* texture, Vector2 startPos, Vector2 sliceSize, Vector2 step, float duration, UINT frameCount, bool repeat, Vector2 animOffset);
	void CreateRigidBody();
	void CreateGravity();

	void SaveAnimation(const wchar_t* animName);

public:
	Collider* GetCollider() { return p_collider; }
	Animator* GetAnimator() { return p_animator; }
	RigidBody* GetRigidBody() { return p_rigidBody; }
	Gravity* GetGravity() { return p_gravity; }

public:
	virtual void OnCollisionEnter(Collider* other) {}
	virtual void OnCollisionStay(Collider* other) {}
	virtual void OnCollisionExit(Collider* other) {}

public:
	virtual void SetPos(Vector2 pos) { _pos = pos; }
	virtual void SetScale(Vector2 scale) { _scale = scale; }
	void SetTheta(float theta) { _theta = theta; }
	void SetDir(Vector2 dir) { _dir = dir; _dir.Normalize(); }
	void SetObjectName(const wstring& name) { _objectName = name; }

public:
	Vector2 GetPos() { return _pos; }
	Vector2 GetScale() { return _scale; }
	Vector2 GetVector2Dir() { return _dir; }
	float	GetTheta() { return _theta; }
	const wstring& GetObjectName() { return _objectName; }
	
	Texture* GetAnim(const wstring& name)
	{
		Texture* texture = ResourceManager::GetInstance()->LoadTexture(name, L"Textures\\Player\\Animation\\" + name);
		return texture;
	}

	friend class EventManager;
};


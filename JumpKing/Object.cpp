#include "pch.h"
#include "Object.h"

#include "TimeManager.h"
#include "PathManager.h"
#include "KeyManager.h"
#include "CameraManager.h"

#include "Collider.h"
#include "Animator.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Gravity.h"

Object::Object()
	:
	_objectName(),
	_alive(true),
	_theta(0),
	_dir{},
	_pos{},
	_scale{},
	p_collider(nullptr),
	p_animator(nullptr),
	p_rigidBody(nullptr),
	p_gravity(nullptr)
{

}

Object::Object(const Object& origin)
	:
	_objectName(origin._objectName),
	_alive(true),
	_theta(origin._theta),
	p_collider(nullptr),
	p_animator(nullptr),
	p_rigidBody(nullptr),
	p_gravity(nullptr)
{
	// Collider있는 경우만 복사를 받는다.
	if (origin.p_collider)
	{
		p_collider = new Collider(*origin.p_collider);
		p_collider->p_owner = this;
	}

	// TODO : Animator 깊은복사
	if (origin.p_animator)
	{
		p_animator = new Animator(*origin.p_animator);
		p_animator->p_owner = this;
	}

	if (origin.p_gravity)
	{
		p_gravity = new Gravity(*origin.p_gravity);
		p_gravity->p_owner = this;
	}

	if (origin.p_rigidBody)
	{
		p_rigidBody = new RigidBody(*origin.p_rigidBody);
		p_rigidBody->p_owner = this;
	}

}

Object::~Object()
{
	if (nullptr != p_collider)
		delete p_collider;

	if (nullptr != p_animator)
		delete p_animator;

	if (nullptr != p_rigidBody)
		delete p_rigidBody;

	if (nullptr != p_gravity)
		delete p_gravity;
}

void Object::FinalUpdate()
{
	if (p_animator != nullptr)
		p_animator->FinalUpdate();

	if (p_gravity != nullptr)
		p_gravity->FinalUpdate();

	if (p_rigidBody != nullptr)
		p_rigidBody->FinalUpdate();

	if (p_collider != nullptr)
		p_collider->FinalUpdate();
}

void Object::Render(HDC dc)
{
	Vector2 renderPos = CameraManager::GetInstance()->GetRenderPos(_pos);

	Rectangle
	(
		dc,
		static_cast<int>(renderPos._x - _scale._x / 2.f),
		static_cast<int>(renderPos._y - _scale._y / 2.f),
		static_cast<int>(renderPos._x + _scale._x / 2.f),
		static_cast<int>(renderPos._y + _scale._y / 2.f)
	);

	ComponentRender(dc);

}

void Object::ComponentRender(HDC dc)
{
	if (nullptr != p_animator)
		p_animator->Render(dc);

	if (nullptr != p_collider)
		p_collider->Render(dc);
}

void Object::CreateCollider()
{
	p_collider = new Collider;
	p_collider->p_owner = this;
}

void Object::CreateAnimator()
{
	p_animator = new Animator();
	p_animator->p_owner = this;
}

void Object::CreateAnimation(const wstring& animName, Texture* texture, Vector2 startPos, Vector2 sliceSize, Vector2 step, float duration, UINT frameCount, bool repeat, Vector2 animOffset)
{
	CreateAnimator();
	GetAnimator()->CreateAnimation(animName, texture, startPos, sliceSize, step, duration, frameCount);
	GetAnimator()->PlayAnimation(animName, repeat);

	Animation* anim = GetAnimator()->FindAnimation(animName);

	for (UINT i = 0; i < anim->GetMaxFrame(); ++i)
	{
		anim->GetAnimFrame(i)._offset = animOffset + Vector2(3.f, 0.f);
	}
}

void Object::CreateRigidBody()
{
	p_rigidBody = new RigidBody();
	p_rigidBody->p_owner = this;
}

void Object::CreateGravity()
{
	p_gravity = new Gravity();
	p_gravity->p_owner = this;
}

void Object::SaveAnimation(const wchar_t* animName)
{
	wstring path = L"Animation\\Player_" + static_cast<wstring>(animName) + L".anim";
	GetAnimator()->FindAnimation(animName)->SaveAnim(path);
}

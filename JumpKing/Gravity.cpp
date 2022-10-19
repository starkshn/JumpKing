#include "pch.h"
#include "Gravity.h"

#include "Object.h"
#include "RigidBody.h"

bool Gravity::_onLand = false;

Gravity::Gravity()
	:
	p_owner(nullptr),
	_ground(false)
{

}

Gravity::~Gravity()
{

}

void Gravity::FinalUpdate()
{
	p_owner->GetRigidBody()->SetAccelAlpha(Vector2(0.f, 800.f));
}

void Gravity::SetOnGround(bool ground)
{
	_ground = ground;

	if (_ground)
	{
		Vector2 v = p_owner->GetRigidBody()->GetVelocity();

		// 2. 점프킹 처럼
		p_owner->GetRigidBody()->SetVelocity(Vector2(v._x, 0.f));
	}
}

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
	p_owner->GetRigidBody()->SetAccelAlpha(Vector2(0.f, 1400.f));
}

void Gravity::SetOnGround(bool ground)
{
	_ground = ground;

	if (_ground)
	{
		Vector2 v = p_owner->GetRigidBody()->GetVelocity();

		// ÀÏ¹Ý ¶¥À§
		p_owner->GetRigidBody()->SetVelocity(Vector2(0.f, 0.f));

		// ºùÆÇÀ§
		// p_owner->GetRigidBody()->SetVelocity(Vector2(v._x, 0.f));
	}
}

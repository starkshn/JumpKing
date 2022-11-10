#include "pch.h"
#include "RigidBody.h"

#include "TimeManager.h"

#include "Object.h"

RigidBody::RigidBody()
	:
	p_owner(nullptr),
	_mass(1.f),
	_frictCoeff(250.f),
	_maxVelocity(Vector2(400.f, 1200.f))
{

}

RigidBody::~RigidBody()
{

}

void RigidBody::Update()
{

}

void RigidBody::FinalUpdate()
{
	// 순수 힘의 크기
	float force = _force.Length();

	if (0.f != force)
	{
		// 방향
		_force.Normalize();

		float accel = force / _mass;

		_accel = _force * accel;
	}

	// ============================================
	// 힘이 있든 없든  추가 가속도를 계산을 해야한다.
	// ============================================
	_accel += _accelAlpha; // 추가 가속도를 더한다.

	 // 속도 (방향 + 속력) 한번 붙은 속도는 초기화 할 필요가 없다.
	_velocity += _accel * DeltaTime_F;

	// =================
	// 마찰력 적용
	// =================
	// 마찰력에 의한 반대방향으로의 가속도
	if (!_velocity.IsZero())
	{
		Vector2 vel = _velocity;
		vel.Normalize();
		Vector2 friction = (-vel) * _frictCoeff * DeltaTime_F;

		if (_velocity.Length() <= friction.Length())
		{
			// 마찰 가속도가 본래 속도보다 더 큰 경우
			_velocity = Vector2(0.f, 0.f);
		}
		else
		{
			_velocity += friction;
		}
	}

	// 최대속도 제한 (속도 제한 검사)
	if (abs(_maxVelocity._x) < abs(_velocity._x))
	{
		_velocity._x = (_velocity._x / abs(_velocity._x)) * abs(_maxVelocity._x);
	}

	if (abs(_maxVelocity._y) < abs(_velocity._y))
	{
		_velocity._y = (_velocity._y / abs(_velocity._y)) * abs(_maxVelocity._y);
	}

	Move();

	// 힘 초기화
	_force = Vector2(0.f, 0.f);

	// 추가 가속도 누적량 초기화
	_accelAlpha = Vector2(0.f, 0.f);

	// 가속도 초기화
	_accel = Vector2(0.f, 0.f);
}

void RigidBody::Move()
{
	// 이동 속력
	float speed = _velocity.Length();

	if (0.f != speed)
	{
		// 이동 방향
		Vector2 dir = _velocity;
		dir.Normalize();

		Vector2 pos = p_owner->GetPos();
		pos += dir * speed * DeltaTime_F;

		p_owner->SetPos(pos);
	}
}
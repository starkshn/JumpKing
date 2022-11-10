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
	// ���� ���� ũ��
	float force = _force.Length();

	if (0.f != force)
	{
		// ����
		_force.Normalize();

		float accel = force / _mass;

		_accel = _force * accel;
	}

	// ============================================
	// ���� �ֵ� ����  �߰� ���ӵ��� ����� �ؾ��Ѵ�.
	// ============================================
	_accel += _accelAlpha; // �߰� ���ӵ��� ���Ѵ�.

	 // �ӵ� (���� + �ӷ�) �ѹ� ���� �ӵ��� �ʱ�ȭ �� �ʿ䰡 ����.
	_velocity += _accel * DeltaTime_F;

	// =================
	// ������ ����
	// =================
	// �����¿� ���� �ݴ���������� ���ӵ�
	if (!_velocity.IsZero())
	{
		Vector2 vel = _velocity;
		vel.Normalize();
		Vector2 friction = (-vel) * _frictCoeff * DeltaTime_F;

		if (_velocity.Length() <= friction.Length())
		{
			// ���� ���ӵ��� ���� �ӵ����� �� ū ���
			_velocity = Vector2(0.f, 0.f);
		}
		else
		{
			_velocity += friction;
		}
	}

	// �ִ�ӵ� ���� (�ӵ� ���� �˻�)
	if (abs(_maxVelocity._x) < abs(_velocity._x))
	{
		_velocity._x = (_velocity._x / abs(_velocity._x)) * abs(_maxVelocity._x);
	}

	if (abs(_maxVelocity._y) < abs(_velocity._y))
	{
		_velocity._y = (_velocity._y / abs(_velocity._y)) * abs(_maxVelocity._y);
	}

	Move();

	// �� �ʱ�ȭ
	_force = Vector2(0.f, 0.f);

	// �߰� ���ӵ� ������ �ʱ�ȭ
	_accelAlpha = Vector2(0.f, 0.f);

	// ���ӵ� �ʱ�ȭ
	_accel = Vector2(0.f, 0.f);
}

void RigidBody::Move()
{
	// �̵� �ӷ�
	float speed = _velocity.Length();

	if (0.f != speed)
	{
		// �̵� ����
		Vector2 dir = _velocity;
		dir.Normalize();

		Vector2 pos = p_owner->GetPos();
		pos += dir * speed * DeltaTime_F;

		p_owner->SetPos(pos);
	}
}
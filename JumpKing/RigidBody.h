#pragma once

class Object;

class RigidBody
{
private: 
	Object* p_owner;

	Vector2 _force;			// ũ��, ����
	Vector2 _accel;			// ���ӵ� 
	Vector2 _accelAlpha;	// �߰� ���ӵ�
	Vector2 _velocity;		// �ӵ� (ũ�� : �ӷ�, ����)
	Vector2 _maxVelocity;	// �ִ� �ӵ� (ũ�� : �ӷ�, ����)

	float	_mass;			// ����
	float	_frictCoeff;	// ���� ���

public:
	RigidBody();
	~RigidBody();

public:
	void Update();
	void FinalUpdate();

private:
	void Move();

public:
	void AddForce(Vector2 force)
	{
		_force += force;
	}

	// ������ �� ��� �ӷ��� ���� �Լ�
	void AddVelocity(Vector2 velocity) { _velocity += velocity; }

public:
	void SetMess(float mass) { _mass = mass; }
	void SetVelocity(Vector2 velocity) { _velocity = velocity; }
	void SetMaxVelocity(Vector2 maxVelocity) { _maxVelocity = maxVelocity; }
	void SetAccelAlpha(Vector2 accelAlpha) { _accelAlpha = accelAlpha; }

public:
	float	GetMass() { return _mass; }
	Vector2 GetVelocity() { return _velocity; }
	Vector2	GetMaxVelocity() { return _maxVelocity; }
	float	GetSpeed() { return _velocity.Length(); }

	friend class Object;
};


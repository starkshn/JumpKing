#pragma once

class Object;

class RigidBody
{
private: 
	Object* p_owner;

	Vector2 _force;			// 크기, 방향
	Vector2 _accel;			// 가속도 
	Vector2 _accelAlpha;	// 추가 가속도
	Vector2 _velocity;		// 속도 (크기 : 속력, 방향)
	Vector2 _maxVelocity;	// 최대 속도 (크기 : 속력, 방향)

	float	_mass;			// 질량
	float	_frictCoeff;	// 마찰 계수

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

	// 움직일 때 즉시 속력을 위한 함수
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


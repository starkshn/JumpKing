#pragma once

class Object;

class Gravity
{
private:
	Object*		p_owner;
	bool		_ground;

public:
	static bool _onLand;

public:
	Gravity();
	~Gravity();

public:
	void FinalUpdate();

public:
	void SetOnGround(bool ground);

public:
	friend class Object;
};


#pragma once

class Collider;

union COLLIDER_ID
{
	struct
	{
		UINT _leftID;
		UINT _rightID;
	};

	ULONGLONG ID;
};

class ColliderManager
{
	SINGLE(ColliderManager);

private:
	ColliderManager();
	~ColliderManager();

private:
	map<ULONGLONG, bool> _mapCollisionInfo;
	UINT _arrCheck[static_cast<UINT>(GROUP_TYPE::END)];

public:
	void Update();
	void CheckGroup(GROUP_TYPE left, GROUP_TYPE right);
	void ResetGroup();

private:
	void CollisionGroupUpdate(GROUP_TYPE left, GROUP_TYPE right);
	bool IsCollision(Collider* leftCol, Collider* rightCol);



};


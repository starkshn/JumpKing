#pragma once
#include "Scene.h"

class Texture;
class CMonster;

class StartScene : public Scene
{
private:
	Texture*	p_backGroundTexture;

	Vector2		_mouseForcePos;

	bool		_useForce;

	float		_forceCurRadius;
	float		_forceRadius;
	float		_force;

	Vector2		_resolution;

public:
	StartScene();
	virtual ~StartScene() override;

private:
	virtual void Enter(Object* player) override;
	virtual void Exit(Object* player) override;
	virtual void Update() override;
	virtual void Render(HDC dc) override;

public:
	void CreateForce();
};


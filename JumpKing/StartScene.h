#pragma once
#include "Scene.h"

class Texture;
class CMonster;

class StartScene : public Scene
{
private:
	Texture* p_backGroundTexture;

	Vector2		_mouseForcePos;

	bool		_useForce;

	float		_forceCurRadius;
	float		_forceRadius;
	float		_force;

public:
	StartScene();
	virtual ~StartScene() override;

private:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Render(HDC dc) override;
	virtual void Exit() override;

public:
	void CreateForce();
};


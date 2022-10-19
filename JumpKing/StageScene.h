#pragma once
#include "Scene.h"

class StageScene : public Scene
{
private:
	Texture* p_backGroundTexture;

	StageScene* p_vecStages[static_cast<UINT>(SCENE_TYPE::END)];
	
	UINT _stageNum;

public:
	StageScene();
	virtual ~StageScene() override;

private:
	virtual void Enter() override;
	virtual void Update() override;
	virtual void Render(HDC dc) override;
	virtual void Exit() override;

};


#pragma once

#include "Scene.h"

class StageScene;
class Object;

class SceneManager
{
	SINGLE(SceneManager);

private:
	SceneManager();
	~SceneManager();

private:
	Scene* p_scenes[static_cast<unsigned int>(SCENE_TYPE::END)]; // ¸ðµç ¾À µî·Ï
	Scene* p_curScene; // ÇöÀç ¾À

	// Tile
	UINT		_maxTileRow;
	UINT		_maxTileCol;


private:
	void ChangeRealScene(SCENE_TYPE sceneType, Object* player);
	static UINT g_stageNumber;

public:
	void Init();
	void Update();
	void Render(HDC sceneDC);

public:
	void UpStageNum() { g_stageNumber++; }
	void DownStageNum() { g_stageNumber--; }

public:
	// Tile
	void SetTileMaxRow(UINT row) { _maxTileRow = row; }
	void SetTileMaxCol(UINT col) { _maxTileCol = col; }

public:
	Scene* GetCurScene() { return p_curScene; }

	Scene* GetSceneArr(SCENE_TYPE type, Scene* scene)
	{
		if (nullptr != p_scenes[static_cast<UINT>(type)])
			delete scene;
		else
			p_scenes[static_cast<UINT>(type)] = scene;
		
		return p_scenes[static_cast<UINT>(type)];
	}

	// Tile
	UINT GetTileMaxRow() { return _maxTileRow; }
	UINT GetTileMaxCol() { return _maxTileCol; }

	UINT GetStageNum() { return g_stageNumber; }

	Scene* GetSceneArr(SCENE_TYPE type) 
	{
		return p_scenes[static_cast<UINT>(type)];
	}


	friend class EventManager;
};


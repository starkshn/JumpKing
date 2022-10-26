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
	vector<Scene*>	_vecScenes; // ¸ðµç ¾À µî·Ï
	Scene*			p_curScene; // ÇöÀç ¾À

	// Tile
	UINT		_maxTileRow;
	UINT		_maxTileCol;

	// StageNumber
	static UINT g_nextStage;
	static UINT g_staticStage;

private:
	void ChangeRealScene(SCENE_TYPE sceneType, Object* player);

public:
	void Init();
	void Update();
	void Render(HDC sceneDC);

public:
	UINT UpNextStage() { return ++g_nextStage; }
	UINT DownNextStage() { return --g_nextStage; }

public:
	// Tile
	void SetTileMaxRow(UINT row) { _maxTileRow = row; }
	void SetTileMaxCol(UINT col) { _maxTileCol = col; }

	void SetStaticStage(UINT stageNumber) { g_staticStage = stageNumber; }

public:
	Scene* GetCurScene() 
	{
		if (p_curScene != nullptr)
			return p_curScene;
		else
			return nullptr;
	}

	Scene* GetScene(SCENE_TYPE type)
	{
		UINT idx = static_cast<UINT>(type);
		
		if (idx < 0 || idx >= static_cast<UINT>(SCENE_TYPE::END))
		{
			return nullptr;
		}

		return _vecScenes[static_cast<UINT>(type)];
	}

	// Tile
	UINT GetTileMaxRow() { return _maxTileRow; }
	UINT GetTileMaxCol() { return _maxTileCol; }

	UINT GetNextStage() { return g_nextStage; }
	UINT GetStaticStage() { return g_staticStage; }

	friend class EventManager;
};


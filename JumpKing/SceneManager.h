#pragma once

#include "Scene.h"

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
	void ChangeRealScene(SCENE_TYPE sceneType);

public:
	void Init();
	void Update();
	void Render(HDC sceneDC);

public:
	// Tile
	void SetTileMaxRow(UINT row) { _maxTileRow = row; }
	void SetTileMaxCol(UINT col) { _maxTileCol = col; }

public:
	Scene* GetCurScene() { return p_curScene; }

	// Tile
	UINT GetTileMaxRow() { return _maxTileRow; }
	UINT GetTileMaxCol() { return _maxTileCol; }


	friend class EventManager;
};


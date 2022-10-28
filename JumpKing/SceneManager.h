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
	vector<Scene*>	_vecScenes; // 모든 씬 등록
	vector<Object*>	_objects[static_cast<UINT>(GROUP_TYPE::END)]; // 모든 오브젝트
	Scene*			p_curScene; // 현재 씬

	// Tile
	UINT		_maxTileRow;
	UINT		_maxTileCol;

	// StageNumber
	static UINT g_nextStage;
	static UINT g_staticStage;

	vector<Scene*>::iterator _stageIter;
	
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
	void AddObject(Object* obj, GROUP_TYPE type)
	{
		_objects[static_cast<unsigned int>(type)].push_back(obj);	
	}

public:
	// Tile
	void SetTileMaxRow(UINT row) { _maxTileRow = row; }
	void SetTileMaxCol(UINT col) { _maxTileCol = col; }

	void SetStaticStage(UINT stageNumber) { g_staticStage = stageNumber; }

public:
	vector<Object*>& GetObjectsVec(UINT idx) { return _objects[idx]; }

	const vector<Object*>& GetGroupObjects(const GROUP_TYPE& type)
	{
		return _objects[static_cast<UINT>(type)];
	}

	vector<Object*>& GetUIGroups(const GROUP_TYPE& type)
	{
		return _objects[static_cast<UINT>(GROUP_TYPE::UI)];
	}

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

	// vector iter를 Scene에다가 적용
	Scene*	GetSceneByVec() { return *_stageIter; }

	Scene*	UpStageByVec() 
	{ 
		if (_stageIter == _vecScenes.end() || (*_stageIter)->GetStageNumber() >= (UINT)SCENE_TYPE::END - 1)
			return nullptr;
		if (_stageIter != _vecScenes.end())
			return *(++_stageIter);
	}
	Scene* DownStageByVec()
	{
		if ((*_stageIter)->GetStageNumber() <= (UINT)SCENE_TYPE::STAGE_1)
			return nullptr;
		if (_stageIter != _vecScenes.end())
			return *(--_stageIter);
	}
	
	friend class EventManager;
};


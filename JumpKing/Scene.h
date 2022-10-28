#pragma once

class Object;
class Texture;
class Player;

class Scene
{
private:
	// vector<Object*> _objects[static_cast<unsigned int>(GROUP_TYPE::END)]; // 오브젝트들을 저장 및 관리할 벡터를 그룹 갯수만큼 선언
	
	wstring		_sceneName;

	// 각각의 StageScene number를 저장할 변수
	UINT		_stageNumber = 0;

	UINT		_tileXCount = 0;
	UINT		_tileYCount = 0;

	// 컨텐츠 상 그냥 플레이어를 기억하도록 하자.
	Object*		p_player = nullptr;

public:
	Scene();
	virtual ~Scene();

public:
	virtual void Init();
	virtual void Update();
	void FinalUpdate();
	virtual void Render(HDC dc);
	void RenderTile(HDC dc);

	virtual void Enter(Object* player) abstract; // 해당 Scene 에 진입시 호출
	virtual void Exit(Object* player) abstract; // 해당 Scene 을 탈출 시 사용

public:
	void RegisterPlayer(Object* player) { p_player = player; }

	void DeleteGroupObjects(GROUP_TYPE groupType);
	void DeleteAllGroups();
	void CreateTile(UINT xCount, UINT yCount);

public:
	void SetSceneName(wstring strName) { _sceneName = strName; }
	void SetCurPlayer(Object* player) { p_player = player; }
	void SetStageNumber(UINT stageNumber) { _stageNumber = stageNumber; }

public:
	Object* GetCurPlayer() { return p_player; }

	const wstring& GetSceneName() const { return _sceneName; }
	Scene* GetCurrentScene() { return this; }
	UINT GetStageNumber() { return _stageNumber; }

	UINT GetTileX() { return _tileXCount; };
	UINT GetTileY() { return _tileYCount; };

};


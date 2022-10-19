#pragma once

class Object;
class Texture;
class Player;

class Scene
{
private:
	vector<Object*> _objects[static_cast<unsigned int>(GROUP_TYPE::END)]; // 오브젝트들을 저장 및 관리할 벡터를 그룹 갯수만큼 선언
	wstring		_sceneName;
	UINT		_tileXCount;
	UINT		_tileYCount;

	// 컨텐츠 상 그냥 플레이어를 기억하도록 하자.
	Object* p_player;

public:
	Scene();
	virtual ~Scene();

public:
	virtual void Init();
	virtual void Update();
	void FinalUpdate();
	virtual void Render(HDC dc);
	void RenderTile(HDC dc);

	virtual void Enter() abstract; // 해당 Scene 에 진입시 호출
	virtual void Exit() abstract; // 해당 Scene 을 탈출 시 사용

public:
	void AddObject(Object* obj, GROUP_TYPE type)
	{
		_objects[static_cast<unsigned int>(type)].push_back(obj);
	}
	void RegisterPlayer(Object* player) { p_player = player; }


	void DeleteGroupObjects(GROUP_TYPE groupType);
	void DeleteAllGroups();
	void CreateTile(UINT xCount, UINT yCount);

public:
	void SetName(wstring strName) { _sceneName = strName; }
	Vector2 SetUICenterPos(Vector2 parent, Vector2 child);

public:
	const wstring& GetName() const { return _sceneName; }
	const vector<Object*>& GetGroupObjects(const GROUP_TYPE& type)
	{
		return _objects[static_cast<UINT>(type)];
	}
	vector<Object*>& GetUIGroups(const GROUP_TYPE& type)
	{
		return _objects[static_cast<UINT>(GROUP_TYPE::UI)];
	}

	UINT GetTileX() { return _tileXCount; };
	UINT GetTileY() { return _tileYCount; };

	Object* GetCurPlayer() { return p_player; }

public:
	void LoadTile(const wstring& relativePath);

};


#pragma once

class Object;
class Texture;
class Player;

class Scene
{
private:
	vector<Object*> _objects[static_cast<unsigned int>(GROUP_TYPE::END)]; // ������Ʈ���� ���� �� ������ ���͸� �׷� ������ŭ ����
	wstring		_sceneName;
	UINT		_tileXCount;
	UINT		_tileYCount;

	// ������ �� �׳� �÷��̾ ����ϵ��� ����.
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

	virtual void Enter() abstract; // �ش� Scene �� ���Խ� ȣ��
	virtual void Exit() abstract; // �ش� Scene �� Ż�� �� ���

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


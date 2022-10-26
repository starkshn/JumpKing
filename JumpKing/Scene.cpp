#include "pch.h"
#include "Scene.h"

#include "PathManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "CameraManager.h"

#include "Object.h"
#include "Core.h"
#include "Texture.h"
#include "Tile.h"

Scene::Scene()
	:
	_tileXCount(),
	_tileYCount(),
	p_player(nullptr)
{

}

Scene::~Scene()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(GROUP_TYPE::END); ++i)
	{
		for (size_t j = 0; j < _objects[i].size(); ++j)
		{
			if (nullptr != _objects[i][j] || _objects[i].size() != 0)
			{
				UINT curStage = GetStageNumber();
				UINT staticStage = SceneManager::GetInstance()->GetStaticStage();

				if (staticStage != curStage)
				{
					if (_objects[i][j]->GetObjectName() == L"Player")
						break;

					delete _objects[i][j];
				}
				else
				{
					delete _objects[i][j];
					_objects[i][j] = nullptr;
					_objects[i].clear();

					int a = 10;
				}
			}
		}
	}
}

void Scene::Init()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(GROUP_TYPE::END); ++i)
	{
		for (size_t j = 0; j < _objects[i].size(); ++j)
		{
			if (!_objects[i][j]->IsDead())
			{
				_objects[i][j]->Init();
			}
		}
	}
}

void Scene::Update()
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(GROUP_TYPE::END); ++i)
	{
		for (size_t j = 0; j < _objects[i].size(); ++j)
		{
			if (!_objects[i][j]->IsDead())
			{
				_objects[i][j]->Update();
			}
		}
	}
}

void Scene::FinalUpdate()
{
	for (UINT i = 0; i < static_cast<UINT>(GROUP_TYPE::END); ++i)
	{
		for (size_t j = 0; j < _objects[i].size(); ++j)
		{
			_objects[i][j]->FinalUpdate();
		}
	}
}

void Scene::Render(HDC dc)
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; ++i)
	{
		if (i == (UINT)GROUP_TYPE::TILE)
		{
			RenderTile(dc);
			continue;
		}

		vector<Object*>::iterator iter = _objects[i].begin();

		for (iter = _objects[i].begin(); iter != _objects[i].end();)
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->Render(dc);
				++iter;
			}
			else
			{
				iter = _objects[i].erase(iter);
			}
		}
	}
}

void Scene::RenderTile(HDC dc)
{
	const vector<Object*>& vecTile = GetGroupObjects(GROUP_TYPE::TILE);

	Vector2 cameraLookPos = CameraManager::GetInstance()->GetLookPos();
	Vector2 resolution = Core::GetInstance()->GetResolution();

	Vector2 leftTopPos = cameraLookPos - (resolution / 2.f);

	int tileSize = TILE_SIZE;

	int leftTopCol = static_cast<int>(leftTopPos._x / tileSize);
	int leftTopRow = static_cast<int>(leftTopPos._y / tileSize);

	int winTileXCount = static_cast<int>(resolution._x / tileSize) + 1;
	int winTileYCount = static_cast<int>(resolution._y / tileSize) + 1;

	for (int curRow = leftTopRow; curRow < (leftTopRow + winTileYCount); ++curRow)
	{
		for (int curCol = leftTopCol; curCol < (leftTopCol + winTileXCount); ++curCol)
		{
			if (curCol < 0 || _tileXCount <= curCol || curRow < 0 || _tileYCount <= curRow)
			{
				continue;
			}

			int curTileIdx = (_tileXCount * curRow) + curCol;

			vecTile[curTileIdx]->Render(dc);
		}
	}
}

void Scene::DeleteGroupObjects(GROUP_TYPE groupType)
{
	// SafeDeleteVector(_objects[static_cast<UINT>(groupType)]);

	// 밑에처럼 명시적으로 호출하는 것이 원래는 정석이다.
	SafeDeleteVector<Object*>(_objects[static_cast<UINT>(groupType)]);
}

void Scene::DeleteAllGroups()
{
	for (size_t i = 0; i < static_cast<UINT>(GROUP_TYPE::END); ++i)
	{
		if (static_cast<UINT>(GROUP_TYPE::PLAYER) == i)
			continue;
		// 모든 오브젝트 싹다 삭제.
		DeleteGroupObjects(static_cast<GROUP_TYPE>(i));
	}
}

void Scene::CreateTile(UINT xCount, UINT yCount)
{
	DeleteGroupObjects(GROUP_TYPE::TILE);

	_tileXCount = xCount;
	_tileYCount = yCount;

	Texture* tileTexture = ResourceManager::GetInstance()->LoadTexture(L"Tile", L"Textures\\tiles.bmp");

	UINT maxCol = tileTexture->GetWidth() / TILE_SIZE;
	UINT maxRow = tileTexture->GetHeight() / TILE_SIZE;

	SceneManager::GetInstance()->SetTileMaxCol(maxCol);
	SceneManager::GetInstance()->SetTileMaxRow(maxRow);

	for (UINT y = 0; y < yCount; ++y)
	{
		for (UINT x = 0; x < xCount; ++x)
		{
			Tile* tile = new Tile();

			tile->SetPos(Vector2(static_cast<float>(x * TILE_SIZE), static_cast<float>(y * TILE_SIZE)));

			tile->SetTileTexture(tileTexture);

			AddObject(tile, GROUP_TYPE::TILE);
		}
	}
}



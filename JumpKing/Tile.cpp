#include "pch.h"
#include "Tile.h"

#include "CameraManager.h"
#include "SceneManager.h"

#include "Texture.h"

Tile::Tile() : p_tileTexture(nullptr), _tileImageIdx(0)
{
	SetScale(Vector2(TILE_SIZE, TILE_SIZE));
}

Tile::~Tile()
{

}

void Tile::Update()
{

}

void Tile::Render(HDC dc)
{
	if (nullptr == p_tileTexture || -1 == _tileImageIdx)
		return;

	UINT width = p_tileTexture->GetWidth();
	UINT height = p_tileTexture->GetHeight();

	// 타일 갯수가 나온다.
	UINT maxCol = (width / TILE_SIZE);
	// 타일 행 갯수가 나온다. (-1 빼주어야한다. 768px이여야 하는데 767px이라서
	UINT maxRow = (height / TILE_SIZE);

	SceneManager::GetInstance()->SetTileMaxCol(maxCol);
	SceneManager::GetInstance()->SetTileMaxRow(maxRow);

	UINT curRow = static_cast<UINT>(_tileImageIdx / maxCol);
	UINT curCol = static_cast<UINT>(_tileImageIdx % maxRow);

	// 현재 행이 최대행을 넘지않게 하기 위한 예외처리
	if (maxRow <= curRow)
		assert(nullptr);

	Vector2 renderPos = CameraManager::GetInstance()->GetRenderPos(GetPos());
	Vector2 scale = GetScale();

	BitBlt
	(
		dc,
		int(renderPos._x),
		int(renderPos._y),
		int(scale._x),
		int(scale._y),
		p_tileTexture->GetDC(),
		curCol * TILE_SIZE,
		curRow * TILE_SIZE,
		SRCCOPY
	);
}

void Tile::SaveTileInfo(FILE* file)
{
	fwrite(&_tileImageIdx, sizeof(int), 1, file);
}

void Tile::LoadTileInfo(FILE* file)
{
	fread(&_tileImageIdx, sizeof(int), 1, file);
}

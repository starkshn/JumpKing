#include "pch.h"
#include "Texture.h"

#include "Core.h"

Texture::Texture() : _dc(0), _bitMap{ 0 }, _objectInfo{}
{

}

Texture::~Texture()
{
	DeleteDC(_dc);
	DeleteObject(_bitMap);
}

void Texture::Load(const wstring& path)
{
	// ���Ϸ� ���� �ε��� �����͸� ��Ʈ������ ����
	_bitMap = (HBITMAP)LoadImage(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);

	// ��Ʈ�ʰ� ������ DC
	_dc = CreateCompatibleDC(Core::GetInstance()->GetMainDC());

	// ��Ʈ�ʰ� DC����
	HBITMAP prevBit = (HBITMAP)SelectObject(_dc, _bitMap);
	DeleteObject(prevBit);

	// ��Ʈ�� ����
	GetObject(_bitMap, sizeof(BITMAP), &_objectInfo);

	if (_bitMap == nullptr)
		assert(_bitMap);

}

void Texture::Create(UINT width, UINT height)
{
	HDC mainDC = Core::GetInstance()->GetMainDC();

	// Texture�� �����.
	_bitMap = CreateCompatibleBitmap(mainDC, width, height);
	_dc = CreateCompatibleDC(mainDC);

	HBITMAP oldBitMap = (HBITMAP)SelectObject(_dc, _bitMap);
	DeleteObject(oldBitMap);

	// ��Ʈ�� ����
	GetObject(_bitMap, sizeof(BITMAP), &_objectInfo);
}

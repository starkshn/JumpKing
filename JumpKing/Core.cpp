#include "pch.h"
#include "Core.h"

#include "Resource.h"

#include "TimeManager.h"
#include "PathManager.h"
#include "KeyManager.h"
#include "CameraManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "ColliderManager.h"
#include "UIManager.h"
#include "EventManager.h"
#include "ResourceManager.h"

#include "GDI.h"
#include "Texture.h"

Core::Core()
	:
	h_wnd(0),
	h_dc(0),
	_resolution{}
{
	CreateHbrush();
	CreateHpen();
}

Core::~Core()
{
	ReleaseDC(h_wnd, h_dc);

	for (UINT i = 0; i < static_cast<UINT>(HPEN_TYPE::END); ++i)
	{
		DeleteObject(h_pens[i]);
	}

	DestroyMenu(h_menu);
}

int Core::Init(HWND hwnd, POINT resolution)
{
	h_wnd = hwnd;
	_resolution = resolution;

	ChangeWindowSize(Vector2(static_cast<float>(_resolution.x), static_cast<float>(_resolution.y)), false);

	h_menu = LoadMenu(nullptr, MAKEINTRESOURCEW(109));
	h_dc = GetDC(h_wnd);

	_bufferTexture = ResourceManager::GetInstance()->CreateTexture(L"BackBuffer", (UINT)_resolution.x, (UINT)_resolution.y);

	// ====================================
	// Managers Init()
	PathManager::GetInstance()->Init();
	TimeManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	CameraManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();
	SoundManager::GetInstance()->Init();
	// ====================================

	return S_OK;
}

void Core::Progress()
{
	// ====================================
	// Managers Update()
	TimeManager::GetInstance()->Update();
	KeyManager::GetInstance()->Update();
	CameraManager::GetInstance()->Update();

	SceneManager::GetInstance()->Update();

	ColliderManager::GetInstance()->Update();

	UIManager::GetInstance()->Update();
	// ====================================

	Clear();

	SceneManager::GetInstance()->Render(_bufferTexture->GetDC());
	CameraManager::GetInstance()->Render(_bufferTexture->GetDC());

	BitBlt
	(
		h_dc,
		0, 0, 
		_resolution.x, _resolution.y,
		_bufferTexture->GetDC(),
		0, 0, SRCCOPY
	);

	EventManager::GetInstance()->Update();
}

void Core::Clear()
{
	GDI gdi(_bufferTexture->GetDC(), HBRUSH_TYPE::BLACK);
	Rectangle(_bufferTexture->GetDC(), -1, -1, _resolution.x + 1, _resolution.y + 1);
}

void Core::DockMenu()
{
	// 메뉴바 부착
	SetMenu(h_wnd, h_menu);
	ChangeWindowSize(GetResolution(), true);
}

void Core::ReleaseMenu()
{
	// 메뉴바 해제
	SetMenu(h_wnd, nullptr);
	ChangeWindowSize(GetResolution(), false);
}

void Core::ChangeWindowSize(Vector2 resolution, bool menu)
{
	RECT rt =
	{
		0, 0,
		static_cast<LONG>(resolution._x),
		static_cast<LONG>(resolution._y)
	};

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, menu);
	SetWindowPos(h_wnd, nullptr, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);
}

void Core::CreateHbrush()
{
	// hollow brush
	h_brushes[static_cast<UINT>(HBRUSH_TYPE::HOLLOW)] = static_cast<HBRUSH>(GetStockObject(HOLLOW_BRUSH));

	h_brushes[static_cast<UINT>(HBRUSH_TYPE::BLACK)] = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));

	int a = 10;
}

void Core::CreateHpen()
{
	// red, blue, green
	h_pens[static_cast<UINT>(HPEN_TYPE::RED)] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	h_pens[static_cast<UINT>(HPEN_TYPE::GREEN)] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	h_pens[static_cast<UINT>(HPEN_TYPE::BLUE)] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

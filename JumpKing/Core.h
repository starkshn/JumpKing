#pragma once

class Texture;

class Core
{
	SINGLE(Core);

private:
	Core();
	~Core();

private:
	HWND	h_wnd;
	HDC		h_dc;
	POINT	_resolution;

	//Texture* _bufferTexture; // ����� �ؽ���

	// ���� ����ϴ� GDI Object (Ŀ�ο�����Ʈ)
	HBRUSH	h_brushes[static_cast<UINT>(HBRUSH_TYPE::END)];
	HPEN	h_pens[static_cast<UINT>(HPEN_TYPE::END)];

	HMENU	h_menu;

	Texture* _bufferTexture; // ����� �ؽ���

public:
	int		Init(HWND hwnd, POINT resolution);
	void	Progress();
	void	Clear();

public:
	void DockMenu();
	void ReleaseMenu();
	void ChangeWindowSize(Vector2 resolution, bool menu);

public:
	void CreateHbrush();
	void CreateHpen();

public:
	HWND	GetMainHwnd() { return h_wnd; }
	POINT	GetResolution() { return _resolution; }
	HDC		GetMainDC() { return h_dc; }
	HBRUSH	GetHbrush(HBRUSH_TYPE type) { return h_brushes[static_cast<UINT>(type)]; }
	HPEN	GetHpen(HPEN_TYPE type) { return h_pens[static_cast<UINT>(type)]; }
	

};


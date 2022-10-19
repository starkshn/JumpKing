#pragma once
#pragma once
class GDI
{
private:
	HDC		h_dc;
	HPEN	h_defaultPen;
	HBRUSH	h_defaultBrush;

public:
	GDI(HDC dc, HBRUSH_TYPE type);
	GDI(HDC dc, HPEN_TYPE type);
	~GDI();
};


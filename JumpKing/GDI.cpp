#include "pch.h"
#include "GDI.h"

#include "Core.h"

GDI::GDI(HDC dc, HBRUSH_TYPE type)
	:
	h_dc(dc),
	h_defaultPen(nullptr),
	h_defaultBrush(nullptr)
{
	HBRUSH brush = Core::GetInstance()->GetHbrush(type);
	h_defaultBrush = static_cast<HBRUSH>(SelectObject(dc, brush));
}

GDI::GDI(HDC dc, HPEN_TYPE type)
	:
	h_dc(dc),
	h_defaultPen(nullptr),
	h_defaultBrush(nullptr)
{
	HPEN pen = Core::GetInstance()->GetHpen(type);
	h_defaultPen = static_cast<HPEN>(SelectObject(dc, pen));
}

GDI::~GDI()
{
	SelectObject(h_dc, h_defaultPen);
	SelectObject(h_dc, h_defaultBrush);
}

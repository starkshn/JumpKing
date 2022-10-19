#include "pch.h"
#include "TimeManager.h"

#include "Core.h"

TimeManager::TimeManager()
{

}

TimeManager::~TimeManager()
{

}

void TimeManager::Init()
{
	// 현재 카운트
	QueryPerformanceCounter(&_prevCount);

	// 초당 카운트 횟수
	QueryPerformanceFrequency(&_frequency);
}

void TimeManager::Update()
{
	QueryPerformanceCounter(&_curCount);

	_deltaTime = static_cast<double>(_curCount.QuadPart - _prevCount.QuadPart) / static_cast<double>(_frequency.QuadPart);

	_prevCount = _curCount;

	Render();

#ifdef _DEBUG
	if (_deltaTime > (1. / 60.))
	{
		_deltaTime = 1. / 60.;
	}
#endif
}

void TimeManager::Render()
{
	++_callCount; // 1초가 된 순간의 값을 알아야한다.
	_acc += _deltaTime; // DT누적

	if (_acc >= 1.)
	{
		_fps = _callCount;
		_acc = 0;
		_callCount = 0;

		wchar_t buffer[255] = {};
		swprintf_s(buffer, L"FPS : %d, DT : %f", _fps, _deltaTime);
		SetWindowText(Core::GetInstance()->GetMainHwnd(), buffer);
	}
}

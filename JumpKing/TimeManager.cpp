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
	// ���� ī��Ʈ
	QueryPerformanceCounter(&_prevCount);

	// �ʴ� ī��Ʈ Ƚ��
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
	++_callCount; // 1�ʰ� �� ������ ���� �˾ƾ��Ѵ�.
	_acc += _deltaTime; // DT����

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

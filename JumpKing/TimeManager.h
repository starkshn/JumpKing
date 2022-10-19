#pragma once


class TimeManager
{
	SINGLE(TimeManager);

private:
	TimeManager();
	~TimeManager();

private:
	LARGE_INTEGER _curCount = {};
	LARGE_INTEGER _prevCount = {};
	LARGE_INTEGER _frequency = {};

	double	_deltaTime = 0; // 프레임간의 시간값
	double	_acc = 0;		// 1초 체크를 위한 누적값
	UINT	_callCount = 0;	// update 호출 횟수 체크
	UINT	_fps = 0;		// 초당 호출 횟수

public:
	void Init();
	void Update();
	void Render();

public:
	double GetDeltaTime() { return _deltaTime; }
	float GetDeltaTimeF() { return static_cast<float>(_deltaTime); }

	double GetAccTime() { return _acc; }
};


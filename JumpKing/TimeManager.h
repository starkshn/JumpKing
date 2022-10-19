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

	double	_deltaTime = 0; // �����Ӱ��� �ð���
	double	_acc = 0;		// 1�� üũ�� ���� ������
	UINT	_callCount = 0;	// update ȣ�� Ƚ�� üũ
	UINT	_fps = 0;		// �ʴ� ȣ�� Ƚ��

public:
	void Init();
	void Update();
	void Render();

public:
	double GetDeltaTime() { return _deltaTime; }
	float GetDeltaTimeF() { return static_cast<float>(_deltaTime); }

	double GetAccTime() { return _acc; }
};


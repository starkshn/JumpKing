#pragma once

class Object;
class Texture;

enum class CAMERA_EFFECT
{
	NONE,
	FADE_IN,
	FADE_OUT,
};

struct CameraEffect
{
	float			_duration;		// 효과 지속 시간
	float			_curTime;		// 카메라 효과 현재 진행된 시간
	CAMERA_EFFECT	_effectType;	// 카메라 효과
};

class CameraManager
{
	SINGLE(CameraManager);

private:
	CameraManager();
	~CameraManager();

private:
	Vector2 _lookAtPos;
	Vector2 _corLookPos;
	Vector2 _prevLookPos;

	Object* _targetObject;
	Vector2	_diff;

	float _getTargetTime = 2.f;
	float _getTargetSpeed;
	float _accTime;

	// Cam Effect
	Texture* p_veilTexture;

	list<CameraEffect> _listCameraEffect;

public:
	void Init();
	void Update();
	void Render(HDC dc);

public:
	void CalDiff();

public:
	void SetLookAtPos(Vector2 pos)
	{
		_lookAtPos = pos;
		float moveDis = (_lookAtPos - _prevLookPos).Length();

		_getTargetSpeed = moveDis / _getTargetTime;
		_accTime = 0.f;
	}
	void SetTarget(Object* target) { _targetObject = target; };

public:
	Vector2		GetLookPos() { return _corLookPos; } // 현재 보고있는 위치
	Object*		GetTarget() { return _targetObject; }
	Vector2		GetRenderPos(Vector2 objPos) { return (objPos - _diff); }
	Vector2		GetRealPos(Vector2 renderPos) { return (renderPos + _diff); }

public:
	void FadeIn(float duration)
	{
		CameraEffect cf = {};
		cf._effectType = CAMERA_EFFECT::FADE_IN;
		cf._duration = duration;
		cf._curTime = 0.f;

		_listCameraEffect.push_back(cf);

		if (0.f == duration)
			assert(nullptr);
	}

	void FadeOut(float duration)
	{
		CameraEffect cf = {};
		cf._effectType = CAMERA_EFFECT::FADE_OUT;
		cf._duration = duration;
		cf._curTime = 0.f;

		_listCameraEffect.push_back(cf);

		if (0.f == duration)
			assert(nullptr);
	}
};


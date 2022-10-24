#include "pch.h"
#include "CameraManager.h"

#include "KeyManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"

#include "Core.h"
#include "Object.h"
#include "Texture.h"

CameraManager::CameraManager()
	:
	_lookAtPos{},
	_targetObject(),
	_getTargetTime(1.f),
	_accTime(0.5f),
	p_veilTexture(nullptr),
	_listCameraEffect{}
{

}

CameraManager::~CameraManager()
{

}

void CameraManager::Init()
{
	Vector2 resolution = Core::GetInstance()->GetResolution();

	p_veilTexture = ResourceManager::GetInstance()->CreateTexture(L"CameraVeil", static_cast<UINT>(resolution._x), static_cast<UINT>(resolution._y));
	// 직접만든 경우 픽셀 메모리 버퍼의 초기값이 다 0으로 밀려있다. (건들필요없다)

}

void CameraManager::Update()
{
	if (_targetObject)
	{
		if (_targetObject->IsDead())
		{
			_targetObject = nullptr;
		}
		else
		{
			_lookAtPos = _targetObject->GetPos();
		}
	}

	//if (KEY_HOLD(KEY::UP))
	//	_lookAtPos._y -= 500.f * DeltaTime_F;
	//if (KEY_HOLD(KEY::DOWN))
	//	_lookAtPos._y += 500.f * DeltaTime_F;
	//if (KEY_HOLD(KEY::LEFT))
	//	_lookAtPos._x -= 500.f * DeltaTime_F;
	//if (KEY_HOLD(KEY::RIGHT))
	//	_lookAtPos._x += 500.f * DeltaTime_F;

	// 화면 중앙좌표와 LookAt 좌표간의 차이값계산
	CalDiff();
}

void CameraManager::Render(HDC dc)
{
	if (_listCameraEffect.empty())
		return;

	CameraEffect& cf = _listCameraEffect.front();
	cf._curTime += DeltaTime_F;

	float ratio = 0.f;
	ratio = cf._curTime / cf._duration;

	if (ratio < 0.f)
		ratio = 0.f;
	if (ratio > 1.f)
		ratio = 1.f;

	int alphaValue = 0;

	if (CAMERA_EFFECT::FADE_OUT == cf._effectType)
	{
		alphaValue = static_cast<int>(255.f * ratio);
	}
	else if (CAMERA_EFFECT::FADE_IN == cf._effectType)
	{
		alphaValue = static_cast<int>(255.f * (1.f - ratio));
	}

	BLENDFUNCTION bf = {};

	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = alphaValue; // 고정 알파값. (일단 최대치 줌)

	AlphaBlend
	(
		dc,
		0, 0,
		(int)p_veilTexture->GetWidth(),
		(int)p_veilTexture->GetHeight(),
		p_veilTexture->GetDC(),
		0, 0,
		(int)p_veilTexture->GetWidth(),
		(int)p_veilTexture->GetHeight(),
		bf
	);

	// 진행 시간이 이펙트 최대 지정 시간을 넘어선 경우
	if (cf._curTime > cf._duration)
	{
		// 효과 종료
		_listCameraEffect.pop_front();
	}
}

void CameraManager::CalDiff()
{
	// prevLookAt과 현재 LootAt 의 차이값을 보정해서 현재의 LooAt의 구한다.

	_accTime += DeltaTime_F;

	if (_accTime >= _getTargetTime)
	{
		_corLookPos = _lookAtPos;
	}
	else
	{
		Vector2 lookDir = (_lookAtPos - _prevLookPos);

		if (!lookDir.IsZero())
		{
			_corLookPos = _prevLookPos + lookDir.Normalize() *  _getTargetSpeed * DeltaTime_F;
		}
	}

	// 둘 사이의 차이값을 뺀 것이 _corLookPos가 쫒아가야할 방향이다.

	Vector2 resolution = Core::GetInstance()->GetResolution();
	Vector2 centerPos = resolution / 2.f;

	_diff = _corLookPos - centerPos;
	_prevLookPos = _corLookPos;
}



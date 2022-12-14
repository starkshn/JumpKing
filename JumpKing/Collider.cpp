#include "pch.h"
#include "Collider.h"

#include "CameraManager.h"
#include "SceneManager.h"

#include "Object.h"
#include "Core.h"
#include "Scene.h"
#include "StageScene.h"
#include "GDI.h"

UINT Collider::g_ID = 0;

Collider::Collider()
	:
	_ID(g_ID++),
	p_owner(nullptr),
	_colCount(0),
	_active(true)
{

}

Collider::Collider(const Collider& origin)
	:
	_ID(g_ID++),
	p_owner(nullptr),
	_offsetPos(origin._offsetPos),
	_colliderScale(origin._colliderScale),
	_active(origin._active)
{

}

Collider::~Collider()
{

}

void Collider::FinalUpdate()
{
	// Object의 위치를 따라간다.
	// 따라간다기 보다 바로 위치를 설정한다.
	Vector2 objectPos = p_owner->GetPos();
	_finalPos = objectPos + _offsetPos;

	assert(0 <= _colCount);
}

// ======================================================
// 실제 게임용 Collider Render함수
// ======================================================
void Collider::Render(HDC dc)
{
	bool isRender = SceneManager::GetInstance()->GetColliderIsRender();

	// Ground Render true일 경우
	if (isRender == false)
	{
		HPEN_TYPE pen = HPEN_TYPE::TRANS;

		GDI b(dc, HBRUSH_TYPE::HOLLOW);
		GDI p(dc, pen);

		Vector2 renderPos = CameraManager::GetInstance()->GetRenderPos(_finalPos);
	}
	else
	{
		// Ground Render false일 경우
		HPEN_TYPE pen = HPEN_TYPE::GREEN;

		if (_colCount)
		{
			pen = HPEN_TYPE::RED;
		}

		GDI b(dc, HBRUSH_TYPE::HOLLOW);
		GDI p(dc, pen);

		Vector2 renderPos = CameraManager::GetInstance()->GetRenderPos(_finalPos);

		Rectangle
		(
			dc,
			static_cast<int>(renderPos._x - _colliderScale._x / 2.f),
			static_cast<int>(renderPos._y - _colliderScale._y / 2.f),
			static_cast<int>(renderPos._x + _colliderScale._x / 2.f),
			static_cast<int>(renderPos._y + _colliderScale._y / 2.f)
		);
	}
}

// ======================================================
// 테스트용 Collider Render함수
// ======================================================
//void Collider::Render(HDC dc)
//{
//	HPEN_TYPE pen = HPEN_TYPE::GREEN;
//
//	if (_colCount)
//	{
//		pen = HPEN_TYPE::RED;
//	}
//
//	GDI b(dc, HBRUSH_TYPE::HOLLOW);
//	GDI p(dc, pen);
//
//	Vector2 renderPos = CameraManager::GetInstance()->GetRenderPos(_finalPos);
//
//	Rectangle
//	(
//		dc,
//		static_cast<int>(renderPos._x - _colliderScale._x / 2.f),
//		static_cast<int>(renderPos._y - _colliderScale._y / 2.f),
//		static_cast<int>(renderPos._x + _colliderScale._x / 2.f),
//		static_cast<int>(renderPos._y + _colliderScale._y / 2.f)
//	);
//}

void Collider::OnCollisionEnter(Collider* other)
{
	p_owner->OnCollisionEnter(other);

	++_colCount;
}

void Collider::OnCollisonStay(Collider* other)
{
	p_owner->OnCollisionStay(other);
}

void Collider::OnCollisionExit(Collider* other)
{
	p_owner->OnCollisionExit(other);

	--_colCount;
}

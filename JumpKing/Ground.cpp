#include "pch.h"
#include "Ground.h"

#include "Collider.h"
#include "Gravity.h"
#include "Player.h"
#include "RigidBody.h"

Ground::Ground()
{
	CreateCollider();
}

Ground::~Ground()
{

}

void Ground::Init()
{
	CreateCollider();
	GetCollider()->SetColliderScale(Vector2(GetScale()));
}

void Ground::Update()
{

}

void Ground::OnCollisionEnter(Collider* other)
{
	Object* otherObj = other->GetColliderOwner();
	if (otherObj->GetObjectName() == L"Player")
	{
		otherObj->GetGravity()->SetOnGround(true);
		// otherObj->GetGravity()->_onLand = true;

		Vector2 playerPos = other->GetFinalPos();
		Vector2 playerScale = other->GetColliderScale();

		float playerPX = playerPos._x + (playerScale._x / 2.f);
		float playerMX = playerPos._x - (playerScale._x / 2.f);

		float playerTopY = playerPos._y - (playerScale._y / 2.f);
		float playerBottomY = playerPos._y + (playerScale._y / 2.f);

		Vector2 pos = GetCollider()->GetFinalPos();
		Vector2 scale = GetCollider()->GetColliderScale();

		float scalePX = pos._x + (scale._x / 2.f) + playerScale._x;
		float scaleMX = pos._x - (scale._x / 2.f) - playerScale._x;

		float scaleTopY = pos._y - (scale._y / 2.f);
		float scaleBottomY = pos._y + (scale._y / 2.f);

		if ( (playerTopY + 1 > scaleBottomY))
		{
			if ((playerPX <= scalePX) && (playerMX >= scaleMX))
			{
				// 나(땅 기준)의 나의(땅의) 아랫면에 충돌했을 때
				otherObj->OnDownToUp(true);

				float lenY = abs(playerPos._y - pos._y);
				float valueY = ((playerScale._y / 2.f) + (scale._y / 2.f)) - lenY;
				playerPos = otherObj->GetPos();
				playerPos._y += valueY;

				otherObj->SetPos(playerPos);
			}
			return;
		}
		// + (playerScale._y / 2.f)
		if ( ( (playerBottomY) <= scaleTopY) && (playerPX <= scalePX && playerMX >= scaleMX))
		{
			// 나(땅의)의 땅의 윗면에 충돌했을 때 (땅에 서있을 때)
			float lenY = abs(playerPos._y - pos._y);
			float valueY = ((playerScale._y / 2.f) + (scale._y / 2.f)) - lenY;
			playerPos = otherObj->GetPos();
			playerPos._y -= valueY;
			otherObj->SetPos(playerPos);
			
			return;
		}
		
		if (pos._x - (scale._x / 2.f) >= (playerPX - 1) && abs(playerPos._y - pos._y) < (playerScale._y / 2.f + scale._y / 2.f))
		{
			// 충돌을 했는데 나(땅의) 왼쪽면에 부딪혔을 때
			otherObj->OnLeftRight(true);

			float lenX = abs(playerPos._x - pos._x);
			float valueX = ((playerScale._x / 2.f) + (scale._x / 2.f)) - lenX;

			playerPos = otherObj->GetPos();
			playerPos._x -= lenX;

			otherObj->GetRigidBody()->SetVelocity(Vector2(-500.f, 0.f));
			return;
		}

		if (pos._x + (scale._x / 2.f) <= (playerMX + 1) && abs(playerPos._y - pos._y) < (playerScale._y / 2.f + scale._y / 2.f))
		{
			// 충돌을 했는데 나(땅의) 오른쪽면에  부딪혔을 때
			otherObj->OnLeftRight(true);

			float lenX = abs(playerPos._x - pos._x);
			float valueX = ((playerScale._x / 2.f) + (scale._x / 2.f)) - lenX;

			playerPos = otherObj->GetPos();
			playerPos._x += lenX;

			otherObj->GetRigidBody()->SetVelocity(Vector2(500.f, 0.f));
			return;
		}

	}
}

void Ground::OnCollisionStay(Collider* other)
{
	Object* otherObj = other->GetColliderOwner();

	if (otherObj->GetObjectName() == L"Player")
	{
		otherObj->GetGravity()->SetOnGround(true);
		// otherObj->GetGravity()->_onLand = false;

		Vector2 objPos = other->GetFinalPos();
		Vector2 objScale = other->GetColliderScale();

		Vector2 pos = GetCollider()->GetFinalPos();
		Vector2 scale = GetCollider()->GetColliderScale();

		float len = abs(objPos._y - pos._y);
		float value = (objScale._y / 2.f + scale._y / 2.f) - len;

		// 충돌을 접한 상태를 유지하기 위해 의도적으로 1픽셀을 덜 올려줌.
		objPos = otherObj->GetPos();
		objPos._y -= value;

		otherObj->SetPos(objPos);
	}
}

void Ground::OnCollisionExit(Collider* other)
{
	Object* otherObj = other->GetColliderOwner();

	if (otherObj->GetObjectName() == L"Player")
	{
		otherObj->GetGravity()->SetOnGround(false);
		// otherObj->GetGravity()->_onLand = false;
	}
}

// =========================
// Ground Create Window Proc
// =========================
//INT_PTR __stdcall GroundCreate(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//	
//}
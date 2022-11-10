#include "pch.h"
#include "Ground.h"

#include "Collider.h"
#include "Gravity.h"
#include "Player.h"
#include "RigidBody.h"

Ground::Ground()
{
	
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

void Ground::Render(HDC dc)
{
	ComponentRender(dc);
}

void Ground::OnCollisionEnter(Collider* other)
{
	Object* otherObj = other->GetColliderOwner();
	if (otherObj->GetObjectName() == L"Player")
	{
		Player* player = dynamic_cast<Player*>(otherObj);
		OBJECT_STATE playerCurState = player->GetCurState();

		// =============================================
		// JUMP �ƴ� ��� �浹üũ
		// =============================================
		if (playerCurState == OBJECT_STATE::MOVE || playerCurState == OBJECT_STATE::IDLE)
		{
			return;
		}

		// =============================================
		// JUMP�� ��� �浹üũ
		// =============================================
		if (playerCurState == OBJECT_STATE::JUMP || playerCurState == OBJECT_STATE::FALL)
		{
			CheckColDir(otherObj);
		}
	}
}

void Ground::OnCollisionStay(Collider* other)
{
	Object* otherObj = other->GetColliderOwner();

	if (otherObj->GetObjectName() == L"Player")
	{
		Player* player = dynamic_cast<Player*>(otherObj);
		OBJECT_STATE playerCurState = player->GetCurState();

		CheckColDir(otherObj);
	}
}

void Ground::OnCollisionExit(Collider* other)
{
	
}

void Ground::CheckColDir(Object* otherObj)
{
	Player* player = dynamic_cast<Player*>(otherObj);
	OBJECT_STATE playerCurState = player->GetCurState();

	Vector2 playerPos = otherObj->GetCollider()->GetFinalPos();
	Vector2 playerScale = otherObj->GetCollider()->GetColliderScale();

	Vector2 groundPos = GetCollider()->GetFinalPos();
	Vector2 groundScale = GetCollider()->GetColliderScale();

	float playerRightPosX = playerPos._x + (playerScale._x / 2.f);
	float playerLeftPosX = playerPos._x - (playerScale._x / 2.f);
	float playerTopPosY = playerPos._y - (playerScale._y / 2.f);
	float playerBottomPosY = playerPos._y + (playerScale._y / 2.f);

	float groundRightPosX = groundPos._x + (groundScale._x / 2.f);
	float groundLeftPosX = groundPos._x - (groundScale._x / 2.f);
	float groundTopPosY = groundPos._y - (groundScale._y / 2.f);
	float groundBottomPosY = groundPos._y + (groundScale._y / 2.f);

	// ground ���ʸ� �ε��� ���(�÷��̾ ������ �� ���� ���
	if (playerBottomPosY >= groundTopPosY && playerTopPosY < groundPos._y)
	{
		if (playerRightPosX <= (groundRightPosX + playerScale._x / 2.f) && playerLeftPosX >= groundLeftPosX - (playerScale._x / 2.f))
		{
			// ��(�� ����)�� ����(����) �Ʒ��鿡 �浹���� ��
			PLAYER_COL_INFO info = { false, false, false, true };
			player->SetPlayerColInfo(info);

			otherObj->GetGravity()->SetOnGround(true);

			float lenY = abs(playerPos._y - groundPos._y);
			float valueY = ((playerScale._y / 2.f) + (groundScale._y / 2.f)) - lenY;
			playerPos = otherObj->GetPos();
			playerPos._y -= valueY;

			otherObj->SetPos(playerPos);
			return;
		}
	}

	// ground ���ʸ� �ε��� ���
	if (playerRightPosX >= groundLeftPosX && playerPos._x < groundLeftPosX)
	{
		if (groundTopPosY - (playerScale._y / 2.f) < playerTopPosY && groundBottomPosY + (playerScale._y / 2.f) > playerBottomPosY)
		{
			PLAYER_COL_INFO info = { true, false, false, false };
			player->SetPlayerColInfo(info);

			float lenX = abs(playerPos._x - groundPos._x);
			float valueX = ((playerScale._x / 2.f) + (groundScale._x / 2.f)) - lenX;
			playerPos = otherObj->GetPos();
			playerPos._x -= valueX;

			otherObj->SetPos(playerPos);
			return;
		}
	}

	// ground �����ʸ� �ε��� ���
	if (playerLeftPosX <= groundRightPosX && playerPos._x > groundRightPosX)
	{
		if (groundTopPosY - (playerScale._y / 2.f) < playerTopPosY && groundBottomPosY + (playerScale._y / 2.f) > playerBottomPosY)
		{
			PLAYER_COL_INFO info = { false, true, false, false };
			player->SetPlayerColInfo(info);

			float lenX = abs(playerPos._x - groundPos._x);
			float valueX = ((playerScale._x / 2.f) + (groundScale._x / 2.f)) - lenX;
			playerPos = otherObj->GetPos();
			playerPos._x += valueX;

			otherObj->SetPos(playerPos);
			return;
		}
	}

	// ground �Ʒ��� �ε��� ���
	if (playerTopPosY - 5 < groundBottomPosY && playerPos._y > groundBottomPosY)
	{
		if (playerRightPosX <= (groundRightPosX + playerScale._x / 2.f) && playerLeftPosX >= groundLeftPosX - (playerScale._x / 2.f))
		{
			PLAYER_COL_INFO info = { false, false, true, false };
			player->SetPlayerColInfo(info);

			float lenY = abs(playerPos._y - groundPos._y);
			float valueY = ((playerScale._y / 2.f) + (groundScale._y / 2.f)) - lenY;
			playerPos = otherObj->GetPos();
			playerPos._y += valueY;

			otherObj->SetPos(playerPos);

			return;
		}
	}
}
#include "pch.h"
#include "ColliderManager.h"
#include "SceneManager.h"

#include "Object.h"
#include "Scene.h"
#include "Collider.h"

ColliderManager::ColliderManager()
{

}

ColliderManager::~ColliderManager()
{

}

void ColliderManager::Update()
{
	for (UINT row = 0; row < static_cast<UINT>(GROUP_TYPE::END); ++row)
	{
		for (UINT col = row; col < static_cast<UINT>(GROUP_TYPE::END); ++col)
		{
			if (_arrCheck[row] & (1 << col))
			{
				CollisionGroupUpdate((GROUP_TYPE)row, (GROUP_TYPE)col);
			}
		}
	}
}

void ColliderManager::CheckGroup(GROUP_TYPE left, GROUP_TYPE right)
{
	UINT row = static_cast<UINT>(left);
	UINT col = static_cast<UINT>(right);

	if (col < row)
	{
		UINT temp = static_cast<UINT>(row);
		row = static_cast<UINT>(col);
		col = temp;
	}

	if (_arrCheck[row] & (1 << col))
		_arrCheck[row] &= ~(1 << col);
	else
		_arrCheck[row] |= (1 << col);
}

void ColliderManager::ResetGroup()
{
	memset(_arrCheck, 0, sizeof(UINT) * static_cast<UINT>(GROUP_TYPE::END));
}


void ColliderManager::CollisionGroupUpdate(GROUP_TYPE left, GROUP_TYPE right)
{
	// Scene* curScene = SceneManager::GetInstance()->GetCurScene();
	const vector<Object*>& vecLeft = SceneManager::GetInstance()->GetGroupObjects(left);
	const vector<Object*>& vecRight = SceneManager::GetInstance()->GetGroupObjects(right);
	map<ULONGLONG, bool>::iterator iter;

	for (size_t i = 0; i < vecLeft.size(); ++i)
	{
		// 충돌체를 보유하지 않는 경우
		if (nullptr == vecLeft[i]->GetCollider())
			continue;

		for (size_t j = 0; j < vecRight.size(); ++j)
		{
			// 충돌체를 보유하지 않는 경우 || 나 자신일 경우
			if (nullptr == vecRight[j]->GetCollider() || vecLeft[i] == vecRight[j])
				continue;

			Collider* leftCollider = vecLeft[i]->GetCollider();
			Collider* rightCollider = vecRight[j]->GetCollider();

			// 두 충돌체 조합 아이디 생성
			COLLIDER_ID id;
			id._leftID = leftCollider->GetID();
			id._rightID = rightCollider->GetID();

			// auto iter = _mapCollisionInfo.find(id.ID);
			iter = _mapCollisionInfo.find(id.ID);

			// 충돌 정보가 미 등록 상태인 경우 등록 (충돌하지 않았다 로)
			if (_mapCollisionInfo.end() == iter)
			{
				_mapCollisionInfo.insert(make_pair(id.ID, false));
				iter = _mapCollisionInfo.find(id.ID);
			}

			// 진짜 충돌 검사
			if (IsCollision(leftCollider, rightCollider))
			{
				// 현재 충돌 중이다.

				if (iter->second)
				{
					// 이전에도 충돌하고 있었다. Stay

					// 둘중 한놈 Delete예정일 수 있다.
					if (vecLeft[i]->IsDead() || vecRight[j]->IsDead())
					{
						// 둘중하나가 삭제 예정이라면은, 충돌을 해제시켜준다.
						leftCollider->OnCollisionExit(rightCollider);
						rightCollider->OnCollisionExit(leftCollider);
						iter->second = false;
					}
					else
					{
						// 삭제될 예정이 아닐 경우
						leftCollider->OnCollisonStay(rightCollider);
						rightCollider->OnCollisonStay(leftCollider);
					}
				}
				else
				{
					// 이전에는 충돌하지 않았다. (딱 처음 충돌한 경우) Enter
					if (!(vecLeft[i]->IsDead()) && !(vecRight[j]->IsDead()))
					{
						// 둘중하나가 삭제 예정이라면은, 충돌 하지 않은것으로 취급
						leftCollider->OnCollisionEnter(rightCollider);
						rightCollider->OnCollisionEnter(leftCollider);
						iter->second = true;
					}
					// 일로오면은 그냥 충돌은 없었던 것이다.

				}
			}
			else
			{
				// 현재 충돌중 이지 않다.

				if (iter->second)
				{
					// 이전에는 충돌하고 있었다. Exit
					leftCollider->OnCollisionExit(rightCollider);
					rightCollider->OnCollisionExit(leftCollider);
					iter->second = false;
				}
			}
		}
	}
}

bool ColliderManager::IsCollision(Collider* leftCol, Collider* rightCol)
{
	Vector2 leftPos = leftCol->GetFinalPos();
	Vector2 leftScale = leftCol->GetColliderScale();

	Vector2 rightPos = rightCol->GetFinalPos();
	Vector2 rightScale = rightCol->GetColliderScale();

	if (abs(rightPos._x - leftPos._x) <= (rightScale._x + leftScale._x) / 2.f &&
		abs(rightPos._y - leftPos._y) <= (rightScale._y + leftScale._y) / 2.f)
	{
		return true;
	}

	return false;
}


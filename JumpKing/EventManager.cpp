#include "pch.h"
#include "EventManager.h"

#include "SceneManager.h"
#include "UIManager.h"

#include "Object.h"
#include "Scene.h"

EventManager::EventManager()
{

}

EventManager::~EventManager()
{

}

void EventManager::Update()
{
	// ================================================
	// 이전 프레임에서 등록해둔 Dead Object 들을 삭제한다.
	// ================================================
	for (size_t i = 0; i < _vecDead.size(); ++i)
	{
		delete _vecDead[i];
	}

	_vecDead.clear();

	// =============
	// Event 처리
	// =============
	for (size_t i = 0; i < _vecEvents.size(); ++i)
	{
		ExcuteEvent(_vecEvents[i]);
	}

	_vecEvents.clear();
}

void EventManager::ExcuteEvent(const Event& event)
{
	switch (event._eventType)
	{
	case EVENT_TYPE::CREATE_OBJECT:
	{
		// lParam : Object Ptr
		// wParam : Object Type

		Object* newObjPtr = (Object*)event._lParam;
		GROUP_TYPE newObjGroupType = (GROUP_TYPE)event._rParam;

		SceneManager::GetInstance()->GetCurScene()->AddObject(newObjPtr, newObjGroupType);

	}
	break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		// Object를 Dead상태로 변경
		// 삭제 예정 오브젝트들을 모아둔다.
		Object* deadObjPtr = (Object*)event._lParam;

		if (!deadObjPtr->IsDead())
		{
			deadObjPtr->SetDead();
			_vecDead.push_back(deadObjPtr);
		}
	}
	break;
	case EVENT_TYPE::SCENE_CHANGE:
	{
		// _objectPtr : nextScene
		SceneManager::GetInstance()->ChangeRealScene((SCENE_TYPE)event._lParam);

		// 포커스  UI 해제 (이전 Scene의 FocusUI를 들고 있기 때문에)
		UIManager::GetInstance()->SetFocusUI(nullptr);
	}
	break;
	}
}



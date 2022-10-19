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
	// ���� �����ӿ��� ����ص� Dead Object ���� �����Ѵ�.
	// ================================================
	for (size_t i = 0; i < _vecDead.size(); ++i)
	{
		delete _vecDead[i];
	}

	_vecDead.clear();

	// =============
	// Event ó��
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
		// Object�� Dead���·� ����
		// ���� ���� ������Ʈ���� ��Ƶд�.
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

		// ��Ŀ��  UI ���� (���� Scene�� FocusUI�� ��� �ֱ� ������)
		UIManager::GetInstance()->SetFocusUI(nullptr);
	}
	break;
	}
}



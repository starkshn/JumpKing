#include "pch.h"
#include "UIManager.h"

#include "SceneManager.h"
#include "KeyManager.h"
#include "Scene.h"
#include "UI.h"

UIManager::UIManager()
	:
	_focusedUI(nullptr)
{

}

UIManager::~UIManager()
{

}

void UIManager::Update()
{
	_focusedUI = GetFocusUI();

	if (nullptr == _focusedUI)
	{
		return;
	}

	UI* targetUI = GetTargetUI(_focusedUI);

	bool tapLbtn = KEY_TAP(KEY::LBTN);
	bool awayLbtn = KEY_AWAY(KEY::LBTN);

	if (nullptr != targetUI)
	{
		// UI위에있다고 얼려준다.
		targetUI->MouseOn();

		if (tapLbtn)
		{
			targetUI->MouseLbtnDown();
			targetUI->_lbtnDown = true;
		}
		else if (awayLbtn)
		{
			targetUI->MouseLbtnUp();

			if (targetUI->_lbtnDown)
			{
				// 마우스 올라가있는 상태에서 때진것이라면은 클릭이다.
				targetUI->MouseLbtnClick();
			}

			// 왼쪽버튼 때면, 눌렀던 체크를 다시 해제한다.
			targetUI->_lbtnDown = false;
		}
	}
}

void UIManager::SetFocusUI(UI* ui)
{
	// 이미 포커싱 중인 경우 || 포커싱 해제요청한 경우
	if (_focusedUI == ui || nullptr == ui)
	{
		_focusedUI = ui;
		return;
	}

	// 여기서 focusedUI 변경해준다.
	_focusedUI = ui;

	Scene* curScene = SceneManager::GetInstance()->GetCurScene();
	vector<Object*>& vecUI = curScene->GetUIGroups(GROUP_TYPE::UI);

	vector<Object*>::iterator iter = vecUI.begin();

	// 적어도 왼쪽 클릭이 발생했다는 보장이 생긴다.
	for (; iter != vecUI.end(); ++iter)
	{
		if (_focusedUI == *iter)
			break;
	}

	// 벡터 내에서 맨 뒤로 순번교체
	vecUI.erase(iter);
	vecUI.push_back(_focusedUI);
}

UI* UIManager::GetFocusUI()
{
	Scene* curScene = SceneManager::GetInstance()->GetCurScene();
	vector<Object*>& vecUI = curScene->GetUIGroups(GROUP_TYPE::UI);

	bool tapLbtn = KEY_TAP(KEY::LBTN);

	// 기존 포커싱 UI를 받아두고 변경되었는지 확인한다.
	UI* focusedUI = _focusedUI;

	if (!tapLbtn)
	{
		return focusedUI;
	}

	vector<Object*>::iterator targetIter = vecUI.end();
	vector<Object*>::iterator iter = vecUI.begin();

	// 적어도 왼쪽 클릭이 발생했다는 보장이 생긴다.
	for (; iter != vecUI.end(); ++iter)
	{
		if (dynamic_cast<UI*>(*iter)->IsMouseOn())
		{
			targetIter = iter;
		}
	}

	// 이번에 Focus된 UI가 없다.
	if (vecUI.end() == targetIter)
	{
		return nullptr;
	}

	focusedUI = dynamic_cast<UI*>(*targetIter);

	// 벡터 내에서 맨 뒤로 순번교체
	vecUI.erase(targetIter);
	vecUI.push_back(focusedUI);

	return focusedUI;
}

// 부모 UI 내에서 실제로 타겟팅된 UI를 찾아서 반환한다.
UI* UIManager::GetTargetUI(UI* parentUI)
{
	bool awayLbtn = KEY_AWAY(KEY::LBTN);

	// 1. 부모 UI 포함, 모든 자식들을 검사 한다.
	UI* targetUI = nullptr;

	static list<UI*> queue;
	static vector<UI*> vecNoneTargetUI;

	queue.clear();
	vecNoneTargetUI.clear();

	queue.push_back(parentUI);

	while (!queue.empty())
	{
		// queue 데이터 하나 꺼낸다.
		UI* ui = queue.front();
		queue.pop_front();

		if (ui->IsMouseOn())
		{
			if (nullptr != targetUI)
			{
				vecNoneTargetUI.push_back(targetUI);
			}

			targetUI = ui;
		}
		else
		{
			vecNoneTargetUI.push_back(ui);
		}

		const vector<UI*>& vecChild = ui->GetChild();
		for (size_t i = 0; i < vecChild.size(); ++i)
		{
			queue.push_back(vecChild[i]);
		}
	}

	if (awayLbtn)
	{
		for (size_t i = 0; i < vecNoneTargetUI.size(); ++i)
		{
			vecNoneTargetUI[i]->_lbtnDown = false;
		}
	}

	return targetUI;
}

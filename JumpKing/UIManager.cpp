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
		// UI�����ִٰ� ����ش�.
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
				// ���콺 �ö��ִ� ���¿��� �������̶���� Ŭ���̴�.
				targetUI->MouseLbtnClick();
			}

			// ���ʹ�ư ����, ������ üũ�� �ٽ� �����Ѵ�.
			targetUI->_lbtnDown = false;
		}
	}
}

void UIManager::SetFocusUI(UI* ui)
{
	// �̹� ��Ŀ�� ���� ��� || ��Ŀ�� ������û�� ���
	if (_focusedUI == ui || nullptr == ui)
	{
		_focusedUI = ui;
		return;
	}

	// ���⼭ focusedUI �������ش�.
	_focusedUI = ui;

	Scene* curScene = SceneManager::GetInstance()->GetCurScene();
	vector<Object*>& vecUI = curScene->GetUIGroups(GROUP_TYPE::UI);

	vector<Object*>::iterator iter = vecUI.begin();

	// ��� ���� Ŭ���� �߻��ߴٴ� ������ �����.
	for (; iter != vecUI.end(); ++iter)
	{
		if (_focusedUI == *iter)
			break;
	}

	// ���� ������ �� �ڷ� ������ü
	vecUI.erase(iter);
	vecUI.push_back(_focusedUI);
}

UI* UIManager::GetFocusUI()
{
	Scene* curScene = SceneManager::GetInstance()->GetCurScene();
	vector<Object*>& vecUI = curScene->GetUIGroups(GROUP_TYPE::UI);

	bool tapLbtn = KEY_TAP(KEY::LBTN);

	// ���� ��Ŀ�� UI�� �޾Ƶΰ� ����Ǿ����� Ȯ���Ѵ�.
	UI* focusedUI = _focusedUI;

	if (!tapLbtn)
	{
		return focusedUI;
	}

	vector<Object*>::iterator targetIter = vecUI.end();
	vector<Object*>::iterator iter = vecUI.begin();

	// ��� ���� Ŭ���� �߻��ߴٴ� ������ �����.
	for (; iter != vecUI.end(); ++iter)
	{
		if (dynamic_cast<UI*>(*iter)->IsMouseOn())
		{
			targetIter = iter;
		}
	}

	// �̹��� Focus�� UI�� ����.
	if (vecUI.end() == targetIter)
	{
		return nullptr;
	}

	focusedUI = dynamic_cast<UI*>(*targetIter);

	// ���� ������ �� �ڷ� ������ü
	vecUI.erase(targetIter);
	vecUI.push_back(focusedUI);

	return focusedUI;
}

// �θ� UI ������ ������ Ÿ���õ� UI�� ã�Ƽ� ��ȯ�Ѵ�.
UI* UIManager::GetTargetUI(UI* parentUI)
{
	bool awayLbtn = KEY_AWAY(KEY::LBTN);

	// 1. �θ� UI ����, ��� �ڽĵ��� �˻� �Ѵ�.
	UI* targetUI = nullptr;

	static list<UI*> queue;
	static vector<UI*> vecNoneTargetUI;

	queue.clear();
	vecNoneTargetUI.clear();

	queue.push_back(parentUI);

	while (!queue.empty())
	{
		// queue ������ �ϳ� ������.
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

#pragma once
#include "Object.h"

class UI : public Object
{
	virtual UI* Clone() = 0;

private:
	vector<UI*> _vecChildUI; // 깊은 복사 진행 해야함.
	UI* p_parentUI;

	Vector2		_finalPos;

	bool		_cameraAffected; 
	bool		_onMouseCheck;
	bool		_lbtnDown;

public:
	UI(bool cameraAffected);
	UI(const UI& origin);
	virtual ~UI() override;

public:
	virtual void Update() override;
	virtual void FinalUpdate() override;
	virtual void Render(HDC dc) override;

	void UpdateChild();
	void FinalUpdateChild();
	void RenderChild(HDC dc);

public:
	void OnMouseCheck();
	bool IsMouseOn() { return _onMouseCheck; };
	bool IsLbtnDown() { return _lbtnDown; }

public:
	virtual void MouseOn();
	virtual void MouseLbtnDown();
	virtual void MouseLbtnUp();
	virtual void MouseLbtnClick();

public:
	void AddChild(UI* ui)
	{
		_vecChildUI.push_back(ui);
		ui->p_parentUI = this;
	}

public:
	UI* GetParentUI() { return p_parentUI; }
	Vector2	GetFinalPos() { return _finalPos; }
	const vector<UI*> GetChild() { return _vecChildUI; }

	bool GetLbtnDown() { return _lbtnDown; }
	bool GetIsMouseOn() { return _onMouseCheck; }

	UI* GetFindChild(UI* parentUI, const wstring& childUI)
	{
		for (UINT i = 0; i < parentUI->GetChild().size(); ++i)
		{
			if (parentUI->GetChild()[i]->GetObjectName() == childUI)
			{
				if (parentUI->GetChild()[i] == nullptr)
					assert(nullptr);

				return parentUI->GetChild()[i];
			}
		}

		return nullptr;
	}

	friend class UIManager;
};


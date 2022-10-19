#pragma once

class UI;

class UIManager
{
	SINGLE(UIManager);

private:
	UIManager();
	~UIManager();

private:
	UI* _focusedUI;

public:
	void Update();

public:
	void SetFocusUI(UI* ui);

public:
	UI* GetTargetUI(UI* parentUI);
	UI* GetFocusUI();
};


#include "pch.h"
#include "KeyManager.h"

#include "Core.h"

int g_arrVK[static_cast<int>(KEY::LAST)] =
{
	VK_LEFT, //LEFT,
	VK_UP,//UP,
	VK_DOWN,//DOWN,
	VK_RIGHT, //RIGFHT,
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'Z',
	'X',
	'C',
	'V',
	'B',

	VK_MENU,
	VK_LSHIFT,
	VK_SPACE,
	VK_CONTROL,
	VK_RETURN,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,

};

KeyManager::KeyManager()
{

}

KeyManager::~KeyManager()
{

}

void KeyManager::Init()
{
	for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
	{
		_vecKey.push_back(KeyInfo{ KEY_STATE::NONE, false });
	}
}

void KeyManager::Update()
{
	HWND hWnd = GetFocus();

	if (nullptr != hWnd) // 윈도우 포커싱 중일 떄 키 이벤트 동작
	{
		for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
		{
			// 키가 눌려있다.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (_vecKey[i]._isPrevInput)
				{
					// 이전에도 눌려있다.
					_vecKey[i]._state = KEY_STATE::HOLD;
				}
				else
				{
					// 이전에 눌려있지 않았다.
					_vecKey[i]._state = KEY_STATE::TAP;
				}
				_vecKey[i]._isPrevInput = true;
			}
			else // 키가 안 눌려있다.
			{
				if (_vecKey[i]._isPrevInput) // 지금은 안눌려있는데 이전프레임에 눌려있을 경우
				{
					// 이전에 눌려있다.
					_vecKey[i]._state = KEY_STATE::AWAY;
				}
				else
				{
					// 이전에도 안눌려 있었다.
					_vecKey[i]._state = KEY_STATE::NONE;
				}
				_vecKey[i]._isPrevInput = false;
			}
		}

		// 포커싱 중일때 현재 마우스 좌표 계산
		POINT pos = {};
		GetCursorPos(&pos);

		// 윈도우 좌표 -> 클라 좌표 ( 인자로 현재 윈도우 핸들값 넣는다.)
		ScreenToClient(Core::GetInstance()->GetMainHwnd(), &pos);

		_curMousePos = Vector2(static_cast<float>(pos.x), static_cast<float>(pos.y));
	}
	else // 윈도우 포커싱 해제
	{
		for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
		{
			_vecKey[i]._isPrevInput = false;
			if (_vecKey[i]._state == KEY_STATE::TAP || _vecKey[i]._state == KEY_STATE::HOLD)
			{
				_vecKey[i]._state = KEY_STATE::AWAY;
			}
			else if (_vecKey[i]._state == KEY_STATE::AWAY)
			{
				_vecKey[i]._state = KEY_STATE::NONE;
			}
		}
	}
}

KEY_STATE KeyManager::GetKeyState(KEY key)
{
	return _vecKey[static_cast<int>(key)]._state;
}

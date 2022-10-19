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

	if (nullptr != hWnd) // ������ ��Ŀ�� ���� �� Ű �̺�Ʈ ����
	{
		for (int i = 0; i < static_cast<int>(KEY::LAST); ++i)
		{
			// Ű�� �����ִ�.
			if (GetAsyncKeyState(g_arrVK[i]) & 0x8000)
			{
				if (_vecKey[i]._isPrevInput)
				{
					// �������� �����ִ�.
					_vecKey[i]._state = KEY_STATE::HOLD;
				}
				else
				{
					// ������ �������� �ʾҴ�.
					_vecKey[i]._state = KEY_STATE::TAP;
				}
				_vecKey[i]._isPrevInput = true;
			}
			else // Ű�� �� �����ִ�.
			{
				if (_vecKey[i]._isPrevInput) // ������ �ȴ����ִµ� ���������ӿ� �������� ���
				{
					// ������ �����ִ�.
					_vecKey[i]._state = KEY_STATE::AWAY;
				}
				else
				{
					// �������� �ȴ��� �־���.
					_vecKey[i]._state = KEY_STATE::NONE;
				}
				_vecKey[i]._isPrevInput = false;
			}
		}

		// ��Ŀ�� ���϶� ���� ���콺 ��ǥ ���
		POINT pos = {};
		GetCursorPos(&pos);

		// ������ ��ǥ -> Ŭ�� ��ǥ ( ���ڷ� ���� ������ �ڵ鰪 �ִ´�.)
		ScreenToClient(Core::GetInstance()->GetMainHwnd(), &pos);

		_curMousePos = Vector2(static_cast<float>(pos.x), static_cast<float>(pos.y));
	}
	else // ������ ��Ŀ�� ����
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

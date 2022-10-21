#include "pch.h"
#include "Func.h"

#include "EventManager.h"


void CreateObjectEvent(Object* objectPtr, GROUP_TYPE groupType)
{
	Event evt = {};
	evt._eventType = EVENT_TYPE::CREATE_OBJECT;
	evt._lParam = (DWORD_PTR)objectPtr;
	evt._rParam = (DWORD_PTR)groupType;
	evt.p_obj = nullptr;

	EventManager::GetInstance()->AddEvent(evt);
}

void DeleteObjectEvent(Object* objectPtr)
{
	Event evt = {};
	evt._eventType = EVENT_TYPE::DELETE_OBJECT;
	evt._lParam = (DWORD_PTR)objectPtr;
	evt.p_obj = nullptr;

	EventManager::GetInstance()->AddEvent(evt);
}

void ChangeScene(SCENE_TYPE changeSceneType, Object* obj)
{
	// 이번프레임에서는 이벤트 등록을 하고 다음 프레임에 씬을 변경을 할 것이다.
	Event evt = {};
	evt._eventType = EVENT_TYPE::SCENE_CHANGE;
	evt._lParam = (DWORD_PTR)changeSceneType;
	evt.p_obj = obj;

	EventManager::GetInstance()->AddEvent(evt);
}

void SaveWString(const wstring& str, FILE* file)
{
	const wchar_t* filePtr = str.c_str();
	size_t len = str.length();

	fwrite(&len, sizeof(size_t), 1, file);

	fwrite(filePtr, sizeof(wchar_t), len, file);
}

void LoadWString(wstring& str, FILE* file)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, file);

	wchar_t buffer[256] = {};
	fread(buffer, sizeof(wchar_t), len, file);

	str = buffer;
}

void FScanf(char* buffer, FILE* file)
{
	int i = 0;
	while (true)
	{
		char c = static_cast<char>(getc(file));
		if (c == '\n')
		{
			buffer[i++] = '\0';
			break;
		}
		buffer[i++] = c;
	}
}


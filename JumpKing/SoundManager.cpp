#include "pch.h"
#include "SoundManager.h"

#include "ResourceManager.h"

#include "Core.h"
#include "Sound.h" 

SoundManager::SoundManager()
	: _sound(nullptr)
	, p_BGM(nullptr)
{

}

SoundManager::~SoundManager()
{

}

int SoundManager::Init(void)
{
	if (FAILED(DirectSoundCreate8(NULL, &_sound, NULL)))
	{
		MessageBox(NULL, L"�������̽���������", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	// ���� ����̽� �������� ����.
	HWND hWnd = Core::GetInstance()->GetMainHwnd();
	if (FAILED(_sound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) // Flag�� ����
	{
		MessageBox(NULL, L"�������̽� �������� ����", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	return true;
}

void SoundManager::RegisterToBGM(Sound* _pSound)
{
	if (p_BGM != nullptr)
		p_BGM->Stop(true);

	p_BGM = _pSound;
}

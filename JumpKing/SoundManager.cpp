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
		MessageBox(NULL, L"사운드디바이스생성실패", L"SYSTEM ERROR", MB_OK);
		return false;
	}

	// 사운드 디바이스 협조레벨 설정.
	HWND hWnd = Core::GetInstance()->GetMainHwnd();
	if (FAILED(_sound->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE))) // Flag값 정리
	{
		MessageBox(NULL, L"사운드디바이스 협조레벨 설정", L"SYSTEM ERROR", MB_OK);
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

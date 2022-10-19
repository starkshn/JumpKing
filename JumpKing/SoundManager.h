#pragma once


// sound 용도 - 헤더 순서 중요
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>

// 코드
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

class Sound;

class SoundManager
{
	SINGLE(SoundManager);

private:
	SoundManager();
	~SoundManager();

private:
	LPDIRECTSOUND8	_sound;	// 사운드카드 대표 객체
	Sound* p_BGM;	// 현재 지정된 BGM Sound

public:
	int Init();
	LPDIRECTSOUND8 GetSoundDevice() { return _sound; }
	void RegisterToBGM(Sound* _pSound);
};


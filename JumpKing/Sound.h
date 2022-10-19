#pragma once
#include "Resources.h"

#include "SoundManager.h"

class Sound : public Resources
{
private:
	LPDIRECTSOUNDBUFFER _soundBuffer; // Sound 버퍼 메모리를 저장할 수 있는 공간 (사운드 카드쪽 메모리임 == 소리 저장할 공간)
	DSBUFFERDESC		_bufferInfo;
	int					_volume;

public:
	Sound();
	~Sound();

public:
	int		Load(const wstring& path);
	void	Play(bool loop = false);
	void	PlayBGM(bool loop = false);
	void	Stop(bool reset = false);

public:
	void	SetVolume(float volume);
	void	SetPosition(float pos);

private:
	bool	LoadWaveSound(const wstring& strPath);
	int		GetDecibel(float volume);
};


#pragma once
#include "Resources.h"

#include "SoundManager.h"

class Sound : public Resources
{
private:
	LPDIRECTSOUNDBUFFER _soundBuffer; // Sound ���� �޸𸮸� ������ �� �ִ� ���� (���� ī���� �޸��� == �Ҹ� ������ ����)
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


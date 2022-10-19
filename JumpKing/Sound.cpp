#include "pch.h"
#include "Sound.h"

#include "ResourceManager.h"
#include "SoundManager.h"

Sound::Sound()
	:
	_soundBuffer(nullptr),
	_bufferInfo{},
	_volume(0)
{

}

Sound::~Sound()
{
	if (nullptr != _soundBuffer)
		_soundBuffer->Release();
}

// TODO
int Sound::Load(const wstring& path)
{
	if (nullptr == SoundManager::GetInstance()->GetSoundDevice())
		assert(nullptr); // ���� ��ü �������� ����

	// Ȯ���� �̸� �����ϱ�
	wchar_t szExt[10] = { 0 };
	_wsplitpath_s(path.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 10);

	if (!wcscmp(szExt, L".wav")) // WAV ���� �ε�
	{
		if (false == LoadWaveSound(path))
			assert(nullptr);
	}
	else
		assert(nullptr);

	return S_OK;
}

bool Sound::LoadWaveSound(const wstring& strPath)
{
	HMMIO file; // file Handler

	wstring strFilePath = strPath;

	// CreateFile
	file = mmioOpen((wchar_t*)(strFilePath.c_str()), NULL, MMIO_READ); // wave������ ����.

	if (nullptr == file)
	{
		MessageBox(NULL, L"���� ���ҽ� ��ο� ������ ����", L"���� �ε� ����", MB_OK);
	}

	// Chunk ��ũ ����ü, ���ڿ��� ������ �ν��ؼ� WaveFormat �� ���ۼ��� ������ �о�´�.
	MMCKINFO parent;
	memset(&parent, 0, sizeof(parent));
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmioDescend(file, &parent, NULL, MMIO_FINDRIFF);

	MMCKINFO child;
	memset(&child, 0, sizeof(child));
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	WAVEFORMATEX wft;
	memset(&wft, 0, sizeof(wft));
	mmioRead(file, (char*)&wft, sizeof(wft));

	mmioAscend(file, &child, 0);
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmioDescend(file, &child, &parent, MMIO_FINDCHUNK);

	memset(&_bufferInfo, 0, sizeof(DSBUFFERDESC));
	_bufferInfo.dwBufferBytes = child.cksize;
	_bufferInfo.dwSize = sizeof(DSBUFFERDESC);
	_bufferInfo.dwFlags = DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE | DSBCAPS_CTRLVOLUME;
	_bufferInfo.lpwfxFormat = &wft;

	if (FAILED(SoundManager::GetInstance()->GetSoundDevice()->CreateSoundBuffer(&_bufferInfo, &_soundBuffer, NULL)))
	{
		MessageBox(NULL, L"���� ���� ���� ����", L"", MB_OK);
		return false;
	}

	void* write1 = NULL;
	void* write2 = NULL;
	DWORD length1, length2;

	_soundBuffer->Lock(0, child.cksize, &write1, &length1
		, &write2, &length2, 0L);

	if (write1 != nullptr)
		mmioRead(file, (char*)write1, length1);
	if (write2 != nullptr)
		mmioRead(file, (char*)write2, length2);

	_soundBuffer->Unlock(write1, length1, write2, length2);

	mmioClose(file, 0);

	// �ʱ� ���� �������� ����
	SetVolume(50.f);

	return true;
}

void Sound::Play(bool loop)
{
	// Play �Լ��� 1��° 2��° ���ڴ� 0 ���� �̹� ����Ǿ��ִ�.
	// 3��° ������ ���带 �ݺ���� �� ������ �ƴ����� �����Ѵ�.
	_soundBuffer->SetCurrentPosition(0);

	if (loop)
		_soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		_soundBuffer->Play(0, 0, 0);
}

// TODO
void Sound::PlayBGM(bool loop)
{
	SoundManager::GetInstance()->RegisterToBGM(this);

	// Play �Լ��� 1��° 2��° ���ڴ� 0 ���� �̹� ����Ǿ��ִ�.
	// 3��° ������ ���带 �ݺ���� �� ������ �ƴ����� �����Ѵ�.
	if (loop)
		_soundBuffer->Play(0, 0, DSBPLAY_LOOPING);
	else
		_soundBuffer->Play(0, 0, 0);
}

void Sound::Stop(bool reset)
{
	_soundBuffer->Stop();

	if (reset)
		_soundBuffer->SetCurrentPosition(0);
}

// Sound ����ð��� �Ű��ִ� �Լ�
void Sound::SetPosition(float pos)
{
	Stop(true);

	// �����
	DWORD dwBytes = (DWORD)((pos / 100.f) * (float)_bufferInfo.dwBufferBytes);
	_soundBuffer->SetCurrentPosition(dwBytes);

	Play();
}

// TODO
void Sound::SetVolume(float volum)
{
	_volume = GetDecibel(volum);
	_soundBuffer->SetVolume(volum);
}

int Sound::GetDecibel(float volume)
{
	if (volume > 100.f)
		volume = 100.f;
	else if (volume <= 0.f)
		volume = 0.00001f;

	// 1 ~ 100 ���̰��� ���ú� ������ ����
	int iVolume = (LONG)(-2000.0 * log10(100.f / volume));

	if (iVolume < -10000)
		iVolume = -10000;
	return  iVolume;
}

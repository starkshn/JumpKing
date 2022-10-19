#include "pch.h"
#include "ResourceManager.h"

#include "Resources.h"
#include "PathManager.h"
#include "Texture.h"
#include "Sound.h"
#include <algorithm>

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	SafeDeleteMap(_mapTexture);
}

Texture* ResourceManager::CreateTexture(const wstring& key, UINT width, UINT height)
{
	Texture* tex = FindTexture(key);

	if (nullptr != tex)
		return tex;

	tex = new Texture;
	tex->SetKey(key);

	tex->Create(width, height);

	_mapTexture.insert(make_pair(key, tex));

	return tex;
}

Texture* ResourceManager::LoadTexture(const wstring& key, const wstring& path)
{
	Texture* loadObject = FindTexture(key);
	if (nullptr != loadObject)
		return loadObject;

	wstring filePath = PathManager::GetInstance()->GetAbsolutePath();
	filePath += path;

	loadObject = new Texture;
	loadObject->Load(filePath);
	loadObject->SetKey(key);
	loadObject->SetRelativePath(path);

	_mapTexture.insert(make_pair(key, loadObject));

	return loadObject;
}

Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path)
{
	Sound* loadSound = FindSound(key);
	if (nullptr != loadSound)
		return loadSound;

	wstring filePath = PathManager::GetInstance()->GetAbsolutePath();
	filePath += path;

	loadSound = new Sound;
	loadSound->Load(filePath);

	_mapSound.insert(make_pair(key, loadSound));

	return loadSound;
}

Texture* ResourceManager::FindTexture(const wstring& key)
{
	map<wstring, Resources*>::iterator iter = _mapTexture.find(key);
	// auto iter = _mapTexture.find(key);
	if (iter == _mapTexture.end())
		return nullptr;

	return (Texture*)iter->second;
}

Sound* ResourceManager::FindSound(const wstring& key)
{
	map<wstring, Resources*>::iterator iter = _mapSound.find(key);

	if (iter == _mapSound.end())
		return nullptr;
	return (Sound*)iter->second;

	return nullptr;
}

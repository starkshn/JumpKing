#include "pch.h"
#include "PathManager.h"

#include "Core.h"

PathManager::PathManager()
	:
	_absolutePath{},
	_relativePath{}
{

}

PathManager::~PathManager()
{
	
}

void PathManager::Init()
{
	GetCurrentDirectory(255, _absolutePath);

	int len = static_cast<int>(wcslen(_absolutePath));

	for (int i = len - 1; i >= 0; --i)
	{
		if ('\\' == _absolutePath[i])
		{
			_absolutePath[i] = '\0';
			break;
		}
	}

	wcscat_s(_absolutePath, 255, L"\\bin\\Contents\\");

	GetRelativePath(_absolutePath);
}

wstring PathManager::GetRelativePath(const wchar_t* path)
{
	// 전체 경로 : filePath
	wstring filePath = path;

	size_t absLen = wcslen(_absolutePath);
	size_t fullLen = filePath.length();

	wstring relativePath = filePath.substr(absLen, fullLen - absLen);

	return relativePath;
}

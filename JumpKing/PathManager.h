#pragma once
class PathManager
{
	SINGLE(PathManager);

private:
	PathManager();
	~PathManager();

private:
	wchar_t _absolutePath[256]; // 절대경로
	wchar_t _relativePath[256]; // 상대경로

public:
	void Init();

public:
	const wchar_t* GetAbsolutePath() { return _absolutePath; }
	wstring GetRelativePath(const wchar_t* path);

};


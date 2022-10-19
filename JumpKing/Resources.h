#pragma once
class Resources
{
private:
	wstring _strKey; // 리소스 키
	wstring _strRelativePath; // 리소스 상대경로

public:
	Resources();
	virtual ~Resources();

public:
	void SetKey(const wstring& strkey) { _strKey = strkey; }
	void SetRelativePath(const wstring& strRelativePath) { _strRelativePath = strRelativePath; }

	const wstring& GetKey() { return _strKey; }
	const wstring& GetRelativePath() { return _strRelativePath; }
};


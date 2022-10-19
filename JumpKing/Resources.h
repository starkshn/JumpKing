#pragma once
class Resources
{
private:
	wstring _strKey; // ���ҽ� Ű
	wstring _strRelativePath; // ���ҽ� �����

public:
	Resources();
	virtual ~Resources();

public:
	void SetKey(const wstring& strkey) { _strKey = strkey; }
	void SetRelativePath(const wstring& strRelativePath) { _strRelativePath = strRelativePath; }

	const wstring& GetKey() { return _strKey; }
	const wstring& GetRelativePath() { return _strRelativePath; }
};


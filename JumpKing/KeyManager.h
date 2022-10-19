#pragma once

enum class KEY_STATE
{
	NONE, 
	TAP,
	HOLD,
	AWAY,
};

enum class KEY
{
	LEFT,
	UP,
	DOWN,
	RIGHT,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	Z,
	X,
	C,
	V,
	B,

	ALT,
	LSHIFT,
	SPACE,
	CTRL,
	ENTER,
	ESC,

	LBTN,
	RBTN,

	LAST,
};

struct KeyInfo
{
	KEY_STATE _state;
	bool _isPrevInput;
};

class KeyManager
{
	SINGLE(KeyManager);

private:
	KeyManager();
	~KeyManager();
	
private:
	vector<KeyInfo> _vecKey;
	Vector2	_curMousePos;

public:
	void Init();
	void Update();

public:
	KEY_STATE GetKeyState(KEY k);
	Vector2 GetCurMousePos() { return _curMousePos; }
};


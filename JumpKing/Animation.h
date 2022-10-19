#pragma once

class Animator;
class Texture;

struct AnimFrame
{
	Vector2		_leftTop;
	Vector2		_sliceSize;
	Vector2		_offset; // offset위치
	float		_duration;
};


class Animation
{
private:
	Animator* p_animator;
	wstring	_animationName;
	Texture* p_texture = nullptr;
	vector<AnimFrame>	_vecAnimFrame = {};

	int	_curFrame;
	float _accTime; 
	bool _animFinish; 

public:
	Animation();
	~Animation();

public:
	void Update();
	void Render(HDC dc);

public:
	void Create(Texture* texture, Vector2 startPos, Vector2 sliceSize, Vector2 step, float duration, UINT frameCount);
	bool IsFinish() { return _animFinish; }

public:
	void SaveAnim(const wstring& relativePath);
	void LoadAnim(const wstring& relativePath);

private:
	// 같은 이름 중복 방지를 위해 private
	void SetAnimName(const wstring& animationName) { _animationName = animationName; }
	void SetAnimFrame(int frame)
	{
		_animFinish = false;
		_curFrame = frame;
		_accTime = 0.f;
	}

public:
	void SetAnimator(Animator* animator) { p_animator = animator; }

public:
	const wstring& GetAnimName() { return _animationName; }
	Animator* GetAnimator() { return p_animator; }
	AnimFrame& GetAnimFrame(int idx) { return _vecAnimFrame[idx]; }
	UINT GetMaxFrame() { return (UINT)_vecAnimFrame.size(); }

	friend class Animator;
};


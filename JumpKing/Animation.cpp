#include "pch.h"
#include "Animation.h"

#include "TimeManager.h"
#include "CameraManager.h"
#include "PathManager.h"
#include "ResourceManager.h"

#include "Animator.h"
#include "Texture.h"
#include "Object.h"

Animation::Animation()
	:
	p_animator(nullptr),
	_animationName(),
	_curFrame(0),
	p_texture(nullptr),
	_accTime(0.f),
	_animFinish(false)
{

}

Animation::~Animation()
{

}

void Animation::Update()
{
	if (_animFinish)
		return;

	_accTime += DeltaTime_F; // 시간누적

	if (_accTime > _vecAnimFrame[_curFrame]._duration)
	{
		++_curFrame;

		if (_vecAnimFrame.size() <= _curFrame)
		{
			_curFrame = -1;
			_animFinish = true;
			_accTime = 0.f;
			return;
		}

		_accTime = _accTime - _vecAnimFrame[_curFrame]._duration;
	}
}

void Animation::Render(HDC dc)
{
	if (_animFinish)
		return;

	Object* go = p_animator->GetOwnerObject();
	Vector2 pos = go->GetPos();
	pos += _vecAnimFrame[_curFrame]._offset; // Object Position에 Offset만큼 추가 이동위치, 연산자 오버로딩

	// 렌더링 좌표로 변환
	pos = CameraManager::GetInstance()->GetRenderPos(pos);

	TransparentBlt
	(
		dc,
		(int)(pos._x - (_vecAnimFrame[_curFrame]._sliceSize._x / 2.f)),
		(int)(pos._y - (_vecAnimFrame[_curFrame]._sliceSize._y / 2.f)),
		(int)(_vecAnimFrame[_curFrame]._sliceSize._x),
		(int)(_vecAnimFrame[_curFrame]._sliceSize._y),
		p_texture->GetDC(),
		(int)(_vecAnimFrame[_curFrame]._leftTop._x),
		(int)(_vecAnimFrame[_curFrame]._leftTop._y),
		(int)(_vecAnimFrame[_curFrame]._sliceSize._x),
		(int)(_vecAnimFrame[_curFrame]._sliceSize._y),
		RGB(255, 0, 255)
	);
}

void Animation::Create
(
	Texture* texture,
	Vector2 startPos, Vector2 sliceSize, Vector2 step,
	float duration, UINT frameCount)
{
	p_texture = texture;

	AnimFrame frm = {};
	// frameCount : 만들어내야할 프레임 갯수
	for (UINT i = 0; i < frameCount; ++i)
	{
		frm._duration = duration;
		frm._sliceSize = sliceSize;
		frm._leftTop = startPos + step * (float)i;

		_vecAnimFrame.push_back(frm);
	}
}

void Animation::SaveAnim(const wstring& relativePath)
{
	wstring filePath = PathManager::GetInstance()->GetAbsolutePath();
	filePath += relativePath; // contents 경로 + 상대경로 => 최종 경로

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"wb");
	assert(file);

	fprintf(file, "[Animation Name]\n");
	string strAnimName = string(_animationName.begin(), _animationName.end());
	fprintf(file, strAnimName.c_str());
	fprintf(file, "\n");


	fprintf(file, "[Texture Name(Key)]\n");
	string strKey = string(p_texture->GetKey().begin(), p_texture->GetKey().end());
	fprintf(file, strKey.c_str());
	fprintf(file, "\n");


	fprintf(file, "[Texture Relative Path]\n");
	string strTexturePath = string(p_texture->GetRelativePath().begin(), p_texture->GetRelativePath().end());
	fprintf(file, strTexturePath.c_str());
	fprintf(file, "\n");


	fprintf(file, "[Anim FrameCount Data]\n");
	fprintf(file, "%d\n", static_cast<int>(_vecAnimFrame.size()));

	// 2-2-3 프레임 정보
	for (size_t i = 0; i < _vecAnimFrame.size(); ++i)
	{
		fprintf(file, "[Frame Index]\n");
		fprintf(file, "%d\n", static_cast<int>(i));
		fprintf(file, "LeftTopPos(Vector2)\n");
		fprintf(file, "%d %d \n", static_cast<int>(_vecAnimFrame[i]._leftTop._x), static_cast<int>(_vecAnimFrame[i]._leftTop._y));

		_vecAnimFrame[i]._offset;

		fprintf(file, "SliceSize(Vector2)\n");
		fprintf(file, "%d %d \n", static_cast<int>(_vecAnimFrame[i]._sliceSize._x), static_cast<int>(_vecAnimFrame[i]._sliceSize._y));

		fprintf(file, "Offset(Vector2)\n");
		fprintf(file, "%d %d \n", static_cast<int>(_vecAnimFrame[i]._offset._x), static_cast<int>(_vecAnimFrame[i]._offset._y));

		fprintf(file, "Duration(float)\n");
		fprintf(file, "%f", static_cast<float>(_vecAnimFrame[i]._duration));

		fprintf(file, "\n");
	}

	fclose(file);
}

void Animation::LoadAnim(const wstring& relativePath)
{
	wstring filePath = PathManager::GetInstance()->GetAbsolutePath();
	filePath += relativePath;

	FILE* file = nullptr;
	_wfopen_s(&file, filePath.c_str(), L"rb");
	assert(file);

	char buffer[256] = {};

	// [Animation Name]
	string str;
	FScanf(buffer, file); // 필드 읽어옴
	FScanf(buffer, file); // 이름 읽어옴
	str = buffer;
	_animationName = wstring(str.begin(), str.end());

	// [Texture Name]
	FScanf(buffer, file); // 필드 읽어옴 Texture 필드
	FScanf(buffer, file); // 이름 읽어옴
	str = buffer;
	wstring strTexKey = wstring(str.begin(), str.end());

	// [FrameCount Data]
	FScanf(buffer, file); // 필드 읽어옴 Texture 필드
	FScanf(buffer, file); // 상대경로 읽어옴
	str = buffer;
	wstring strTexRelativePath = wstring(str.begin(), str.end());

	// 있으면 찾아서 반환하고 없다면 로딩한다.
	p_texture = ResourceManager::GetInstance()->LoadTexture(strTexKey, strTexRelativePath);

	FScanf(buffer, file); // [Anim FrameCount Data]
	int frameCount = 0;
	fscanf_s(file, "%d", &frameCount);

	// [5. 프레임 정보 가져오기]
	AnimFrame frm = {};
	for (int i = 0; i < frameCount; ++i)
	{
		POINT pt = {};

		while (true)
		{
			FScanf(buffer, file);

			if (!strcmp("[Frame Index]", buffer))
			{
				fscanf_s(file, "%d", &pt.x);
			}
			else if (!strcmp("LeftTopPos(Vector2)", buffer))
			{
				fscanf_s(file, "%d", &pt.x);
				fscanf_s(file, "%d", &pt.y);

				frm._leftTop = Vector2(pt);
			}
			else if (!strcmp("SliceSize(Vector2)", buffer))
			{
				fscanf_s(file, "%d", &pt.x);
				fscanf_s(file, "%d", &pt.y);

				frm._sliceSize = Vector2(pt);
			}
			else if (!strcmp("Offset(Vector2)", buffer))
			{
				fscanf_s(file, "%d", &pt.x);
				fscanf_s(file, "%d", &pt.y);

				frm._offset = Vector2(pt);
			}
			else if (!strcmp("Duration(float)", buffer))
			{
				fscanf_s(file, "%f", &frm._duration);
				break;
			}
		}

		_vecAnimFrame.push_back(frm);
	}


	fclose(file);
}

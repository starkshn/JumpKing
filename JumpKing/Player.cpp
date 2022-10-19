#include "pch.h"
#include "Player.h"

#include "KeyManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "PathManager.h"

#include "Collider.h"
#include "Animation.h"
#include "Animator.h"
#include "RigidBody.h"
#include "Gravity.h"

Player::Player()
	:
	_curState(OBJECT_STATE::IDLE),
	_prevState(OBJECT_STATE::IDLE),
	_dir(1),
	_prevDir(1),
	_onJump(false)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vector2{ 0.f, 0.f });
	GetCollider()->SetColliderScale(Vector2{ 90.f, 103.f });

	SetObjectName(L"Player");

	// IDLE 가로 : 32, 세로 39.2: 
	// 텍스쳐 로딩
	Texture* p_textureRight = GetAnim(L"PlayerAnimation_R.bmp");
	Texture* p_textureLeft = GetAnim(L"PlayerAnimation_L.bmp");

	CreateAnimator();

	/*GetAnimator()->LoadAnimation(L"Animation\\Player_IDLE_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_IDLE_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_MOVE_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_MOVE_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_JUMP_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_JUMP_LEFT.anim");*/

#pragma region "Animation 저장전"// 372

		// 가로 93, 세로 103
		GetAnimator()->CreateAnimation(L"IDLE_RIGHT", p_textureRight, Vector2(0, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
		GetAnimator()->CreateAnimation(L"IDLE_LEFT", p_textureLeft, Vector2(837, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 1);
	
		// Move
		GetAnimator()->CreateAnimation(L"MOVE_RIGHT", p_textureRight, Vector2(93, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 3);
		GetAnimator()->CreateAnimation(L"MOVE_LEFT", p_textureLeft, Vector2(744, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 3);
	
	    // Sqaut
		GetAnimator()->CreateAnimation(L"SQUAT_RIGHT", p_textureRight, Vector2(372, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
		GetAnimator()->CreateAnimation(L"SQUAT_LEFT", p_textureLeft, Vector2(465, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 1);
	
		// Jump
		GetAnimator()->CreateAnimation(L"JUMP_RIGHT", p_textureRight, Vector2(465, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
		GetAnimator()->CreateAnimation(L"JUMP_LEFT", p_textureLeft, Vector2(372, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 1);

		// Off
		GetAnimator()->CreateAnimation(L"OFF_RIGHT", p_textureRight, Vector2(558, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
		GetAnimator()->CreateAnimation(L"OFF_LEFT", p_textureLeft, Vector2(279, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 1);

		// Fall
		GetAnimator()->CreateAnimation(L"FALL_RIGHT", p_textureRight, Vector2(651, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
		GetAnimator()->CreateAnimation(L"FALL_LEFT", p_textureLeft, Vector2(186, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 1);

		// Fallen
		GetAnimator()->CreateAnimation(L"FALLEN_RIGHT", p_textureRight, Vector2(744, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
		GetAnimator()->CreateAnimation(L"FALLEN_LEFT", p_textureLeft, Vector2(93, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 1);

	#pragma endregion
	
		GetAnimator()->FindAnimation(L"IDLE_RIGHT")->SaveAnim(L"Animation\\Player_IDLE_RIGHT.anim");
		GetAnimator()->FindAnimation(L"IDLE_LEFT")->SaveAnim(L"Animation\\Player_IDLE_LEFT.anim");
		GetAnimator()->FindAnimation(L"MOVE_RIGHT")->SaveAnim(L"Animation\\Player_MOVE_RIGHT.anim");
		GetAnimator()->FindAnimation(L"MOVE_LEFT")->SaveAnim(L"Animation\\Player_MOVE_LEFT.anim");
		GetAnimator()->FindAnimation(L"JUMP_RIGHT")->SaveAnim(L"Animation\\Player_JUMP_RIGHT.anim");
		GetAnimator()->FindAnimation(L"JUMP_LEFT")->SaveAnim(L"Animation\\Player_JUMP_LEFT.anim");

	GetAnimator()->PlayAnimation(L"IDLE_RIGHT", true);

	CreateRigidBody();
	CreateGravity();
}

Player::~Player()
{

}

void Player::Update()
{
	UpdateState();
	UpdateMove();
	UpdateAnimation();

	_prevState = _curState;
	_prevDir = _dir;

	// 중력 최대 가속도 테스트를 위한 재배치
	if (KEY_TAP(KEY::R))
	{
		SetPos(Vector2(640.f, 500.f));
	}
}

void Player::Render(HDC dc)
{
	Object::ComponentRender(dc);
}

void Player::UpdateState()
{
	if (KEY_HOLD(KEY::A))
	{
		_dir = -1;
		if (OBJECT_STATE::JUMP != _curState)
			_curState = OBJECT_STATE::MOVE;
	}
	if (KEY_HOLD(KEY::D))
	{
		_dir = 1;
		if (OBJECT_STATE::JUMP != _curState)
			_curState = OBJECT_STATE::MOVE;
	}

	if (0.f == GetRigidBody()->GetSpeed() && OBJECT_STATE::JUMP != _curState)
	{
		if (KEY_NONE(KEY::A) && KEY_NONE(KEY::D) && KEY_NONE(KEY::SPACE))
		{
			_curState = OBJECT_STATE::IDLE;
		}
	}

	if (KEY_TAP(KEY::SPACE))
	{
		if (_onJump == false)
		{
			_curState = OBJECT_STATE::JUMP;

			if (GetRigidBody())
			{
				// ===================
				// 이단 점프 구현하고 싶을 경우
				/*Vector2 curVel = GetRigidBody()->GetVelocity();
				GetRigidBody()->SetVelocity(Vector2(curVel._x, -200.f));*/
				// ===================

				GetRigidBody()->AddVelocity(Vector2(0.f, -200.f));
			}
		}
	}
}

void Player::UpdateMove()
{

}

void Player::UpdateAnimation()
{

}

void Player::UpdateGravity()
{

}

void Player::OnCollisionEnter(Collider* other)
{

}

void Player::OnCollisionStay(Collider* other)
{

}

void Player::OnCollisionExit(Collider* other)
{

}

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
#include "Sound.h"
#include "Ground.h"

Player::Player()
	:
	_curState(OBJECT_STATE::IDLE),
	_prevState(OBJECT_STATE::IDLE),
	_dir(1),
	_prevDir(1)
{
	CreateCollider();
	GetCollider()->SetOffsetPos(Vector2{ 0.f, 10.f });
	GetCollider()->SetColliderScale(Vector2{ 60.f, 80.f });

	SetObjectName(L"Player");

	// IDLE 가로 : 32, 세로 39.2: 
	// 텍스쳐 로딩
	Texture* p_textureRight = GetAnim(L"JumpKing_R.bmp");
	Texture* p_textureLeft = GetAnim(L"JumpKing_L.bmp");

	CreateAnimator();

	GetAnimator()->LoadAnimation(L"Animation\\Player_IDLE_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_IDLE_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_MOVE_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_MOVE_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_SQUAT_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_SQUAT_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_JUMP_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_JUMP_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_FALL_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_FALL_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_FALLEN_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_FALLEN_LEFT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_OFF_RIGHT.anim");
	GetAnimator()->LoadAnimation(L"Animation\\Player_OFF_LEFT.anim");

#pragma region "애니매이션 로드 및 파일로 저장"
	// ====================================================================
	// 애니매이션 Load 부분
	// 가로 93, 세로 103

	// IDLE
	//GetAnimator()->CreateAnimation(L"IDLE_RIGHT", p_textureRight, Vector2(0, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//GetAnimator()->CreateAnimation(L"IDLE_LEFT", p_textureLeft, Vector2(744, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//
	//// MOVE
	//GetAnimator()->CreateAnimation(L"MOVE_RIGHT", p_textureRight, Vector2(93, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 3);
	//GetAnimator()->CreateAnimation(L"MOVE_LEFT", p_textureLeft, Vector2(465, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 3);
	//
	//// Squat
	//GetAnimator()->CreateAnimation(L"SQUAT_RIGHT", p_textureRight, Vector2(372, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//GetAnimator()->CreateAnimation(L"SQUAT_LEFT", p_textureLeft, Vector2(372, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//
	//// Jump
	//GetAnimator()->CreateAnimation(L"JUMP_RIGHT", p_textureRight, Vector2(465, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//GetAnimator()->CreateAnimation(L"JUMP_LEFT", p_textureLeft, Vector2(279, 0), Vector2(93, 103), Vector2(-93, 0), 0.1f, 1);

	//// Off
	//GetAnimator()->CreateAnimation(L"OFF_RIGHT", p_textureRight, Vector2(558, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//GetAnimator()->CreateAnimation(L"OFF_LEFT", p_textureLeft, Vector2(186, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);

	//// Fall
	//GetAnimator()->CreateAnimation(L"FALL_RIGHT", p_textureRight, Vector2(651, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//GetAnimator()->CreateAnimation(L"FALL_LEFT", p_textureLeft, Vector2(93, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);

	//// Fallen
	//GetAnimator()->CreateAnimation(L"FALLEN_RIGHT", p_textureRight, Vector2(744, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	//GetAnimator()->CreateAnimation(L"FALLEN_LEFT", p_textureLeft, Vector2(0, 0), Vector2(93, 103), Vector2(93, 0), 0.1f, 1);
	////// ====================================================================


	////// ====================================================================
	//// 애니매이션 파일로 저장하는 부분
	//GetAnimator()->FindAnimation(L"IDLE_RIGHT")->SaveAnim(L"Animation\\Player_IDLE_RIGHT.anim");
	////GetAnimator()->FindAnimation(L"IDLE_LEFT")->SaveAnim(L"Animation\\Player_IDLE_LEFT.anim");
	//SaveAnimation(L"IDLE_LEFT");

	//GetAnimator()->FindAnimation(L"MOVE_RIGHT")->SaveAnim(L"Animation\\Player_MOVE_RIGHT.anim");
	//GetAnimator()->FindAnimation(L"MOVE_LEFT")->SaveAnim(L"Animation\\Player_MOVE_LEFT.anim");

	//GetAnimator()->FindAnimation(L"SQUAT_RIGHT")->SaveAnim(L"Animation\\Player_SQUAT_RIGHT.anim");
	//GetAnimator()->FindAnimation(L"SQUAT_LEFT")->SaveAnim(L"Animation\\Player_SQUAT_LEFT.anim");

	//GetAnimator()->FindAnimation(L"JUMP_RIGHT")->SaveAnim(L"Animation\\Player_JUMP_RIGHT.anim");
	//GetAnimator()->FindAnimation(L"JUMP_LEFT")->SaveAnim(L"Animation\\Player_JUMP_LEFT.anim");

	//GetAnimator()->FindAnimation(L"OFF_RIGHT")->SaveAnim(L"Animation\\Player_OFF_RIGHT.anim");
	//GetAnimator()->FindAnimation(L"OFF_LEFT")->SaveAnim(L"Animation\\Player_OFF_LEFT.anim");

	//GetAnimator()->FindAnimation(L"FALL_RIGHT")->SaveAnim(L"Animation\\Player_FALL_RIGHT.anim");
	//GetAnimator()->FindAnimation(L"FALL_LEFT")->SaveAnim(L"Animation\\Player_FALL_LEFT.anim");

	//GetAnimator()->FindAnimation(L"FALLEN_RIGHT")->SaveAnim(L"Animation\\Player_FALLEN_RIGHT.anim");
	// GetAnimator()->FindAnimation(L"FALLEN_LEFT")->SaveAnim(L"Animation\\Player_FALLEN_LEFT.anim");
	// ====================================================================
#pragma endregion

	GetAnimator()->PlayAnimation(L"IDLE_RIGHT", true);

	// =================================================================
	// Sound Load
	ResourceManager::GetInstance()->LoadSound(L"JUMP", L"Sounds\\jump.wav");
	p_jump = ResourceManager::GetInstance()->FindSound(L"JUMP");
	p_jump->SetPosition(50.f);

	ResourceManager::GetInstance()->LoadSound(L"LAND", L"Sounds\\land.wav");
	p_land = ResourceManager::GetInstance()->FindSound(L"LAND");
	p_land->SetPosition(50.f);

	ResourceManager::GetInstance()->LoadSound(L"BUMP", L"Sounds\\bump.wav");
	p_bump = ResourceManager::GetInstance()->FindSound(L"BUMP");
	p_bump->SetPosition(50.f);

	ResourceManager::GetInstance()->LoadSound(L"FALL", L"Sounds\\fall.wav");
	p_fall = ResourceManager::GetInstance()->FindSound(L"FALL");
	p_fall->SetPosition(50.f);
	// =================================================================

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

	// Player 위치 초기화
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
	if (_curState == OBJECT_STATE::FALL || _curState == OBJECT_STATE::OFF || _curState == OBJECT_STATE::JUMP)
		return;

	if (_curState == OBJECT_STATE::JUMP)
	{
		// 점프 하면은 방향 -1
		Vector2 dir = GetRigidBody()->GetVelocity();
		float f = dir.Length();

		if (f < 4.f && f > 1.f)
		{
			_curState = OBJECT_STATE::FALL;
		}
	}

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
		SetState(OBJECT_STATE::SQUAT);
	}

	if (KEY_HOLD(KEY::SPACE))
	{
		SetState(OBJECT_STATE::SQUAT);
		GetCollider()->SetOffsetPos(Vector2{ 0.f, 30.f });
		GetCollider()->SetColliderScale(Vector2{ 60.f, 40.f });
			
		_acc += DeltaTime_F;
		
	}
	else if (KEY_AWAY(KEY::SPACE))
	{
		GetCollider()->SetOffsetPos(Vector2{ 0.f, 10.f });
		GetCollider()->SetColliderScale(Vector2{ 60.f, 80.f });

		_curState = OBJECT_STATE::JUMP;

		p_jump->Play();

		bool left = false;
		bool right = false;

		if (KEY_HOLD(KEY::A))
			left = true;
		if (KEY_HOLD(KEY::D))
			right = true;

		if (GetRigidBody())
		{
			// ===================
			// 이단 점프 구현하고 싶을 경우
			/*Vector2 curVel = GetRigidBody()->GetVelocity();
			GetRigidBody()->SetVelocity(Vector2(curVel._x, -200.f));*/
			// ===================

			_jumpLevel = GetJumpLevel(_acc);

			if (left)
			{
				GetRigidBody()->AddVelocity(Vector2(_dir * 300.f, -2200.f / _jumpLevel));
			}

			if (right)
			{
				GetRigidBody()->AddVelocity(Vector2(_dir * 300.f, -2200.f / _jumpLevel));
			}

			GetRigidBody()->AddVelocity(Vector2(0.f, -2200.f / _jumpLevel));
		}
	}
}

void Player::UpdateMove()
{
	if (_curState == OBJECT_STATE::JUMP || _curState == OBJECT_STATE::SQUAT || _curState == OBJECT_STATE::FALL || _curState == OBJECT_STATE::OFF)
		return;

	RigidBody* rd = GetRigidBody();

	if (KEY_HOLD(KEY::A))
	{
		// rd->AddForce(Vector2(-200.f, 0.f));
		rd->SetVelocity(Vector2(-300.f, rd->GetVelocity()._y));
	}
	if (KEY_HOLD(KEY::D))
	{
		// rd->AddForce(Vector2(200.f, 0.f));
		rd->SetVelocity(Vector2(300.f, rd->GetVelocity()._y));
	}

	if (KEY_TAP(KEY::A))
	{
		// rd->AddVelocity(Vector2(-200.f, 0.f));
		rd->SetVelocity(Vector2(-300.f, rd->GetVelocity()._y));

	}
	if (KEY_TAP(KEY::D))
	{
		// rd->AddVelocity(Vector2(200.f, 0.f));
		rd->SetVelocity(Vector2(300.f, rd->GetVelocity()._y));
	}
}

void Player::UpdateAnimation()
{
	if (_prevState == _curState && _prevDir == _dir)
		return;

	switch (_curState)
	{
	case OBJECT_STATE::IDLE:
	{
		if (_dir == 1)
			GetAnimator()->PlayAnimation(L"IDLE_RIGHT", true);
		else
			GetAnimator()->PlayAnimation(L"IDLE_LEFT", true);
	}
	break;
	case OBJECT_STATE::MOVE:
	{
		if (_dir == 1)
			GetAnimator()->PlayAnimation(L"MOVE_RIGHT", true);
		else
			GetAnimator()->PlayAnimation(L"MOVE_LEFT", true);
	}
	break;
	case OBJECT_STATE::SQUAT:
	{
		if (_dir == 1)
			GetAnimator()->PlayAnimation(L"SQUAT_RIGHT", true);
		else
			GetAnimator()->PlayAnimation(L"SQUAT_LEFT", true);
	}
	break;
	case OBJECT_STATE::JUMP:
	{
		if (_dir == 1)
			GetAnimator()->PlayAnimation(L"JUMP_RIGHT", true);
		else
			GetAnimator()->PlayAnimation(L"JUMP_LEFT", true);
	}
	break;
	case OBJECT_STATE::OFF:
	{
		if (_dir == 1)
			GetAnimator()->PlayAnimation(L"OFF_RIGHT", true);
		else
			GetAnimator()->PlayAnimation(L"OFF_LEFT", true);
	}
	break;
	case OBJECT_STATE::FALL:
	{
		if (_dir == 1)
			GetAnimator()->PlayAnimation(L"FALL_RIGHT", true);
		else
			GetAnimator()->PlayAnimation(L"FALL_LEFT", true);
	break;
	}
	case OBJECT_STATE::FALLEN:
	{
		if (_dir == 1)
			GetAnimator()->PlayAnimation(L"FALLEN_RIGHT", true);
		else
			GetAnimator()->PlayAnimation(L"FALLEN_LEFT", true);
	}
	break;
	case OBJECT_STATE::END:
	{
	}
	break;
	default:
	break;
	}
}

void Player::UpdateGravity()
{
	GetRigidBody()->AddForce(Vector2(0.f, 500.f));
}

void Player::OnCollisionEnter(Collider* other)
{
	Object* otherObj = other->GetColliderOwner();
	if (other->GetColliderOwner()->GetObjectName() == L"Ground")
	{
		_onTop = CheckColDir(otherObj);
		OBJECT_STATE playerState = GetCurState();

		// result == true라면은 땅에 착지한 경우
		if (_onTop)
		{
			p_land->Play();

			if (playerState == OBJECT_STATE::JUMP)
			{
				SetState(OBJECT_STATE::IDLE);
			}
			
			if (playerState == OBJECT_STATE::FALL || playerState == OBJECT_STATE::OFF)
			{
				SetState(OBJECT_STATE::FALLEN);
			}
		}
		else
		{
			Vector2 curVelocity = GetRigidBody()->GetVelocity();
			Vector2 percentValue = VelocityToPercent(curVelocity);
			GetRigidBody()->SetVelocity(Vector2(0.f, 0.f));

			// JUMP, FALL OFF 일 경우 Enter발생하면 무조건 OFF
			if (playerState == OBJECT_STATE::JUMP)
			{
				p_bump->Play();
				
				if (_dir == -1)
				{
					GetRigidBody()->AddVelocity(Vector2(_dir * -1 * (percentValue._x + 200.f), curVelocity._y / _jumpLevel));
				}

				if (_dir == 1)
				{
					GetRigidBody()->AddVelocity(Vector2(_dir * ((percentValue._x + 200.f) * -1), curVelocity._y / _jumpLevel));
				}
				
				SetState(OBJECT_STATE::OFF);
			}

			if (playerState == OBJECT_STATE::FALL)
			{

			}

			if (playerState == OBJECT_STATE::OFF)
			{
				p_bump->Play();

				if (_dir == -1)
				{
					GetRigidBody()->AddVelocity(Vector2((_dir * -1) * (200.f),  200.f * -1));
				}

				if (_dir == 1)
				{
					GetRigidBody()->AddVelocity(Vector2(_dir * 200.f * -1, 200.f * -1));
				}
			}

			_jumpLevel = 0;
		}
	}
}

void Player::OnCollisionStay(Collider* other)
{
	/*OBJECT_STATE playerState = GetCurState();
	PLAYER_COL_INFO info;

	Object* otherObj = other->GetColliderOwner();
	if (other->GetColliderOwner()->GetObjectName() == L"Ground")
	{
		CheckColDir(otherObj);
	}*/
}

void Player::OnCollisionExit(Collider* other)
{
	if (other->GetColliderOwner()->GetObjectName() == L"Ground")
	{
		OBJECT_STATE playerState = GetCurState();

		if (_onTop)
		{
			if (playerState == OBJECT_STATE::MOVE || playerState == OBJECT_STATE::IDLE)
				SetState(OBJECT_STATE::FALL);
		}

		_onTop = false;
	}
}

bool Player::CheckColDir(Object* otherObj)
{
	OBJECT_STATE playerState = GetCurState();

	Ground* ground = dynamic_cast<Ground*>(otherObj);

	Vector2 playerPos = GetCollider()->GetFinalPos();
	Vector2 playerScale = GetCollider()->GetColliderScale();

	Vector2 groundPos = otherObj->GetCollider()->GetFinalPos();
	Vector2 groundScale = otherObj->GetCollider()->GetColliderScale();

	float playerRightPosX = playerPos._x + (playerScale._x / 2.f);
	float playerLeftPosX = playerPos._x - (playerScale._x / 2.f);
	float playerTopPosY = playerPos._y - (playerScale._y / 2.f);
	float playerBottomPosY = playerPos._y + (playerScale._y / 2.f);

	float groundRightPosX = groundPos._x + (groundScale._x / 2.f);
	float groundLeftPosX = groundPos._x - (groundScale._x / 2.f);
	float groundTopPosY = groundPos._y - (groundScale._y / 2.f);
	float groundBottomPosY = groundPos._y + (groundScale._y / 2.f);

	// ground 위쪽면 부딪힐 경우(플레이어가 땅위에 서 있을 경우
	if (playerBottomPosY + 5 >= groundTopPosY && playerPos._y < groundTopPosY)
	{
		if (playerRightPosX <= (groundRightPosX + groundScale._x / 2.f) && playerLeftPosX >= groundLeftPosX - (groundScale._x / 2.f))
		{
			return true;
		}
	}

	// ground 왼쪽면 부딪힐 경우
	if (playerRightPosX  >= groundLeftPosX && playerPos._x < groundLeftPosX)
	{
		if (groundTopPosY - (groundScale._y / 2.f) < playerTopPosY && groundBottomPosY + (groundScale._y / 2.f) > playerBottomPosY)
		{
			return false;
		}
	}

	// ground 오른쪽면 부딪힐 경우
	if (playerLeftPosX <= groundRightPosX && playerPos._x > groundRightPosX)
	{
		if (groundTopPosY - (groundScale._y / 2.f) < playerTopPosY && groundBottomPosY + (groundScale._y / 2.f) > playerBottomPosY)
		{
			return false;
		}
	}

	// ground 아랫면 부딪힐 경우
	if (playerTopPosY <= groundBottomPosY && playerPos._y > groundBottomPosY)
	{
		if (playerRightPosX <= (groundRightPosX + groundScale._x / 2.f) && playerLeftPosX >= groundLeftPosX - (groundScale._x / 2.f))
		{
			return false;
		}
	}
}

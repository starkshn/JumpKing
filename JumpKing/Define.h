#pragma once

#include "pch.h"

#define SINGLE(type) public: static type* GetInstance() { static type mrg; return &mrg; }

#define DeltaTime_F TimeManager::GetInstance()->GetDeltaTimeF()
#define DeltaTime TimeManager::GetInstance()->GetDeltaTime()

#define CLONE(type) type* Clone() { return new type(*this); }

#define KEY_CHECK(key, state) KeyManager::GetInstance()->GetKeyState(key) == state

#define KEY_HOLD(key) KEY_CHECK(key, KEY_STATE::HOLD)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_AWAY(key) KEY_CHECK(key, KEY_STATE::AWAY)
#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define MOUSE_POS KeyManager::GetInstance()->GetCurMousePos()

#define PI 3.1415926535f

#define TILE_SIZE 63
#define BUTTON_SIZE 72

// Button UI Idx
#define TILE_BUTTON 1
#define LEFT_BUTTON 10
#define RIGHT_BUTTON 11
#define CANCLE_BUTTON 15
#define CHECK_BUTTON 16

// PaenlUI Page
#define PAGE_INDEX 35


enum class GROUP_TYPE
{
	DEFAULT,
	TILE,
	PLAYER,
	MONSTER,
	GROUND,

	UI = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,

	END,
};

enum class HBRUSH_TYPE
{
	HOLLOW,
	BLACK,
	END,
};

enum class HPEN_TYPE
{
	RED,
	GREEN,
	BLUE,

	END,
};


enum class EVENT_TYPE
{
	CREATE_OBJECT,
	DELETE_OBJECT,
	SCENE_CHANGE,
	CHANGE_AI_STATE,

	END,
};


enum class MONSTER_STATE
{
	IDLE,
	PATROL,
	TRACE,
	ATTACK,
	MOVE,
	ESCAPE,
	DEAD,

	END = 12
};

// STATE
enum class OBJECT_STATE
{
	IDLE,
	MOVE,
	SQUAT,
	JUMP,
	OFF,
	FALL,
	FALLEN,

	END
};
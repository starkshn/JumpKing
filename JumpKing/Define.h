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
	STAGE_1 = 0,
	STAGE_2,
	STAGE_3,
	STAGE_4,
	STAGE_5,
	STAGE_6,
	STAGE_7,
	STAGE_8,
	STAGE_9,
	STAGE_10,
	STAGE_12,
	STAGE_13,
	STAGE_14,
	STAGE_15,
	STAGE_16,
	STAGE_17,
	STAGE_18,
	STAGE_19,
	STAGE_20,
	STAGE_21,
	STAGE_22,
	STAGE_23,
	STAGE_24,
	STAGE_25,
	STAGE_26,
	STAGE_27,
	STAGE_28,
	STAGE_29,
	STAGE_30,
	STAGE_31,
	STAGE_32,
	STAGE_33,
	STAGE_34,
	STAGE_35,
	STAGE_36,
	STAGE_37,
	STAGE_38,
	STAGE_39,
	STAGE_40,
	STAGE_41,
	STAGE_42,
	STAGE_43 = 42,
	END = 43
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



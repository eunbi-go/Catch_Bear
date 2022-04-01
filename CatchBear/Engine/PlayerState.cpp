#include "pch.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "JumpState.h"

IdleState PlayerState::idle = IdleState();
MoveState PlayerState::move = MoveState();
JumpState PlayerState::jump = JumpState();

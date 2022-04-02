#include "pch.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "MoveState.h"
#include "JumpState.h"
#include "AttackState.h"
#include "DashState.h"
#include "DashRestState.h"

IdleState PlayerState::idle = IdleState();
MoveState PlayerState::move = MoveState();
JumpState PlayerState::jump = JumpState();
AttackState PlayerState::attack = AttackState();
DashState PlayerState::dash = DashState();
DashRestState PlayerState::dashRest = DashRestState();
#include "pch.h"
#include "PlayerState.h"
#include "IdleState.h"
#include "MoveState.h"

IdleState PlayerState::idle = IdleState();
MoveState PlayerState::move = MoveState();

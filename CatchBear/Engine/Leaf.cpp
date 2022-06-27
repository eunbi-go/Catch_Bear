#include "pch.h"
#include "Leaf.h"
#include "Transform.h"
#include "Engine.h"
#include "Timer.h"
#include "GameObject.h"

Leaf::Leaf()
{
}

Leaf::~Leaf()
{
}

void Leaf::Start()
{
	_fallSpeed = 0.05f;
}

void Leaf::LateUpdate()
{
	//// pos
	//Vec3 pos = GetTransform()->GetLocalPosition();
	//pos.y -= _fallSpeed * DELTA_TIME;
	//GetTransform()->SetLocalPosition(pos);
}

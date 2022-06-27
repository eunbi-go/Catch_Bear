#include "pch.h"
#include "Leaf.h"
#include "Transform.h"

Leaf::Leaf()
{
}

Leaf::~Leaf()
{
}

void Leaf::Update()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
}

void Leaf::LateUpdate()
{
}

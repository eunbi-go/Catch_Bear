#include "pch.h"
#include "Leaf.h"
#include "Transform.h"
#include "Engine.h"

Leaf::Leaf()
{
}

Leaf::~Leaf()
{
}

void Leaf::Start()
{
	LeafParticle leafParams = {};
	leafParams.fallSpeed = 4.f;

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::LEAF_PARTICLE)->PushGraphicsData(&leafParams, sizeof(LeafParticle));
}

void Leaf::Update()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
}

void Leaf::LateUpdate()
{
}

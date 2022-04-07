#include "pch.h"
#include "TagMark.h"
#include "Timer.h"
#include "Transform.h"
#include "GameObject.h"
#include "Component.h"

TagMark::TagMark()
{
}

TagMark::~TagMark()
{
}

void TagMark::Update()
{
	Vec3 rotation = GetTransform()->GetLocalRotation();
	rotation.y += DELTA_TIME * _fRotationSpeed;
	GetTransform()->SetLocalRotation(rotation);
}

void TagMark::LateUpdate()
{
	// 술래되면 색 변하게
}

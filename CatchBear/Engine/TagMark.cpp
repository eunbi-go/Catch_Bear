#include "pch.h"
#include "TagMark.h"
#include "Timer.h"
#include "Transform.h"
#include "GameObject.h"
#include "Component.h"
#include "SceneManager.h"
#include "Scene.h"

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

	//_target = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObject(L"Player");
	for (int i = 0; i < g_EnterPlayerCnt; ++i)
	{
		if (GET_SINGLE(SceneManager)->GetActiveScene()->GetPlayer(i)->GetIsTagger() == true)
			_target = GET_SINGLE(SceneManager)->GetActiveScene()->GetPlayer(i);
	}

	Vec3 transform = _target->GetTransform()->GetLocalPosition();
	transform.y += 2.f;
	GetTransform()->SetLocalPosition(transform);
}

void TagMark::LateUpdate()
{
	// 술래되면 색 변하게 해야 함
	// -> 플-플 충돌 후, 서버에서 알려줘야 함
	// -> 그 때 구현할게요
}

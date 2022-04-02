#include "pch.h"
#include "SphereCollider.h"
#include "GameObject.h"
#include "Transform.h"

SphereCollider::SphereCollider() : BaseCollider(ColliderType::Sphere)
{
}

SphereCollider::~SphereCollider()
{
}

void SphereCollider::FinalUpdate()
{
	// bondingSphere 정보를 세팅해줌

	// 충돌 테스트를 할때 어디서 충돌을 할지가 중요하다
	// 광선을 쏘더라도 뷰스페이스에서 표현할 수도 있고, 월드 스페이스나 해당 물체의 로컬 스페이스에서 표현할 수도 있다.
	// 스페이스를 맞춰줘야지만 제대로 된 판정을 할 수 있다.
	// 여기서는 World를 기준으로 생각하고 있다.

	// 멤버변수에 있는 center와 radius는 로컬 기준이긴 한데
	// 얘네들을 FinalUpdate에서 실질적으로 bondingSphere를 체크할 때는
	// SphereCollider가 붙어있는 물체의 월드 포지션을 그대로 가져오고 있다.
	// Scale도 내 스케일의 x, y, z중에서 제일 큰 걸로 인정해서 넣어주고 있다.

	// 지금은 해당 물체의 포지션 자체를 center로 인정할 것이고
	// radius(반경 자체)는 radius에 x, y, z를 곱해서 월드 기준으로 그 아이의 범위를 표현해주고 있다.

	//_boundingSphere.Center = GetGameObject()->GetTransform()->GetWorldPosition();
	_boundingSphere.Center = GetGameObject()->GetTransform()->GetLocalPosition();

	Vec3 scale = GetGameObject()->GetTransform()->GetLocalScale();
	_boundingSphere.Radius = _radius * max(max(scale.x, scale.y), scale.z);
}

bool SphereCollider::Intersects(Vec4 rayOrigin, Vec4 rayDir, OUT float& distance)
{
	// 광선의 시작점과 방향을 넣어줌
	// 충돌했으면 거리가 distance에 들어갈 것임
	return _boundingSphere.Intersects(rayOrigin, rayDir, OUT distance);
}
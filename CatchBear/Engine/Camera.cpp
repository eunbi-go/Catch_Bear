#include "pch.h"
#include "Camera.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Engine.h"

Matrix Camera::S_MatView;
Matrix Camera::S_MatProjection;

Camera::Camera() : Component(COMPONENT_TYPE::CAMERA)
{
}

Camera::~Camera()
{
}

void Camera::FinalUpdate()
{
	// 행렬 계산

	// 뷰행렬: 현재 찍고있는 카메라의 월드행렬의 역행렬
	_matView = GetTransform()->GetLocalToWorldMatrix().Invert();

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	if (_type == PROJECTION_TYPE::PERSPECTIVE)
		_matProjection = ::XMMatrixPerspectiveFovLH(_fov, width / height, _near, _far);
	else
		_matProjection = ::XMMatrixOrthographicLH(width * _scale, height * _scale, _near, _far);

	// 나중에 렌더링하는 부분이 깔끔하게 정리되면 수정 (임시적x, 어디선가 전달받아서)
	S_MatView = _matView;
	S_MatProjection = _matProjection;

	_frustum.FinalUpdate();
}

void Camera::Render()
{
	shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetActiveScene();

	// TODO: Layer 구분
	const vector<shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

	// 모든 오브젝트 렌더 x, MeshRenderer를 들고있는 애들만 렌더해줌
	for (auto& gameObject : gameObjects)
	{
		if (!gameObject->GetMeshRenderer())
			continue;

		// Frustum Culling
		// skybox같은 경우는 어떠한 상황이든 그려줘야 함 -> Frustum culling 하면 안됨 (gameObject에 bool변수 추가)
		if (gameObject->GetCheckFrustum())
		{
			if (_frustum.ContainsSphere(
				gameObject->GetTransform()->GetWorldPosition(),
				gameObject->GetTransform()->GetBoundingSphereRadius()) == false)
			{
				continue;
			}
		}

		gameObject->GetMeshRenderer()->Render();
	}
}

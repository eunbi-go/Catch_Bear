#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"
#include "Timer.h"
#include "AnimationController.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Resources.h"
#include "Shader.h"

void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	// uint64: instanceID
	map<uint64, vector<shared_ptr<GameObject>>> cache;

	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject->_isRender)
		{
			const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
			cache[instanceId].push_back(gameObject);
		}
	}

	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		// 인스턴싱X
		// 1. 플레이어
		if (vec[0]->GetAnimationController() != NULL)
		{
			for (int i = 0; i < vec.size(); ++i)
			{
				// SetInt(0, 0): 인스턴싱 사용X
				vec[i]->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				vec[i]->GetMeshRenderer()->Render();

			}
			continue;
		}

		// 3. 그 외
		else if (vec.size() == 1)
		{
			vec[0]->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
			vec[0]->GetMeshRenderer()->Render();
		}

		// 2. TagMark
		else if (vec[0]->GetName() == L"PlayerTag1" || vec[0]->GetName() == L"PlayerTag2" || vec[0]->GetName() == L"PlayerTag3")
		{
			int tagNum = GET_SINGLE(SceneManager)->GetActiveScene()->_tagNum;

			for (int i = 0; i < vec.size(); ++i)
			{
				if (tagNum == i)
					vec[i]->GetMeshRenderer()->GetMaterial()->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"TagMark"));
				else
					vec[i]->GetMeshRenderer()->GetMaterial()->SetShader(GET_SINGLE(Resources)->Get<Shader>(L"NormalTagMark"));
				vec[i]->GetMeshRenderer()->GetMaterial()->SetInt(0, 0);
				vec[i]->GetMeshRenderer()->Render();
			}
		}

		// 인스턴싱O
		else
		{
			// map<uint64, vector<shared_ptr<GameObject>>> pair
			const uint64 instanceId = pair.first;

			for (const shared_ptr<GameObject>& gameObject : vec)
			{
				InstancingParams params;
				params.matWorld = gameObject->GetTransform()->GetLocalToWorldMatrix();
				params.matWV = params.matWorld * Camera::S_MatView;
				params.matWVP = params.matWorld * Camera::S_MatView * Camera::S_MatProjection;

				AddParam(instanceId, params);
			}

			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
			vec[0]->GetMeshRenderer()->GetMaterial()->SetInt(0, 1);
			vec[0]->GetMeshRenderer()->Render(buffer);
		}
	}
}

void InstancingManager::ClearBuffer()
{
	for (auto& pair : _buffers)
	{
		shared_ptr<InstancingBuffer>& buffer = pair.second;
		buffer->Clear();
	}
}

void InstancingManager::AddParam(uint64 instanceId, InstancingParams& data)
{
	// instanceID에 해당하는 버퍼가 있는지 확인
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}
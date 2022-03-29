#include "pch.h"
#include "InstancingManager.h"
#include "InstancingBuffer.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Transform.h"
#include "Camera.h"

// Render()를 호출하는 순간, 카메라에서 분류를 한 오브젝트를 루프를 돌면서 하나씩 그려주는게 아니라
// InstancingManager에게 렌더링해달라고 모든 물체들을 다 떠넘겨주게 된다.
// 그러면 이 물체들을(gameObjects) 분류를 해야 한다.
// GetInstanceID() 함수를 통해 얘가 그려줘야 하는 인스턴스가 몇번째인지 받아올 것이고
// 그것에 따라서 오브젝트를 분류해서 넣어준다.

void InstancingManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{
	// 같은 instanceID(uint64)에 해당하는 물체들끼리 모아주기 위한 맵을 만듦, 캐시하는 용도
	// 여기에 물체들을 하나씩 다 배치해준다.
	// 물체들은 딱 하나만 그려야 하는 경우도 있을 것이고,
	// 인스턴스 아이디가 겹쳐서 물체가 인스턴싱을 거쳐서 그려줘야 하는 경우로 분류된다.
	map<uint64, vector<shared_ptr<GameObject>>> cache;
	
	for (shared_ptr<GameObject>& gameObject : gameObjects)
	{
		const uint64 instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
		cache[instanceId].push_back(gameObject);
	}

	// 모든 물체를 다 분류했으면 지금까지 저장된 맵에 있는 모든 정보를 돌면서
	for (auto& pair : cache)
	{
		const vector<shared_ptr<GameObject>>& vec = pair.second;

		// 만약 물체가 하나밖에 없다면 기존과 마찬가지로 일반적인 Render() 활용
		if (vec.size() == 1)
		{
			vec[0]->GetMeshRenderer()->Render();
		}

		// 인스턴싱이 적용이 되어야 한다면
		// 모든 물체들을 다 한번에 그려주기 위해서 물체들을 모아주고 있다.
		else
		{
			const uint64 instanceId = pair.first;

			for (const shared_ptr<GameObject>& gameObject : vec)
			{
				// 인스턴싱 파라미터 정보를 월드, 월드뷰, 월드뷰 프로젝션을 각기 실시간으로 만들어주고 있고
				// AddParam()를 통해 이 함수에다가 해당 인스턴스 아이디에 해당하는 파라미터 정보를 추가해달라고 넣어주고 있다.
				// AddParam(): 내가 원하는 인스턴스 아이디에 해당하는 버퍼가 있는지 찾은 다음에
				// 없으면 만들어준다. 만든 다음 여기에 AddData()를 통해서 실제 정보를 추가해준다.
				InstancingParams params;
				params.matWorld = gameObject->GetTransform()->GetLocalToWorldMatrix();
				params.matWV = params.matWorld * Camera::S_MatView;
				params.matWVP = params.matWorld * Camera::S_MatView * Camera::S_MatProjection;

				AddParam(instanceId, params);
			}

			// 인스턴싱 버퍼에 모든 정보들이 채워졌으면
			// 얘들을 한방에 그리게 하기 위해 InstancingBuffer를 넘겨줘서 렌더링하는
			// 두번째 버전인 Render()를 호출해준다.
			// InstancingBuffer를 받아서 한번에 그려주는 역할
			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
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
	if (_buffers.find(instanceId) == _buffers.end())
		_buffers[instanceId] = make_shared<InstancingBuffer>();

	_buffers[instanceId]->AddData(data);
}
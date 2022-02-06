#pragma once
#include "InstancingBuffer.h"

class GameObject;

class InstancingManager
{
	DECLARE_SINGLE(InstancingManager);

public:
	void Render(vector<shared_ptr<GameObject>>& gameObjects);

	void ClearBuffer();
	void Clear() { _buffers.clear(); }

	// 물체의 vertex buffer, index buffer, material이 똑같아서 같은 셰이더로 그려지는 경우 -> 동일하다
	// 동일한 material, mesh 등을 사용하는 물체들끼리 구분할 수 있도록 ID를 지어줌 -> instanceID
private:
	void AddParam(uint64 instanceId, InstancingParams& data);

private:
	map<uint64/*instanceId*/, shared_ptr<InstancingBuffer>> _buffers;
};


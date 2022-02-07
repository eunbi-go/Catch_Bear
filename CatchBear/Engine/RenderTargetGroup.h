#pragma once
#include "Texture.h"

enum class RENDER_TARGET_GROUP_TYPE : uint8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	SHADOW,		// SHADOW
	G_BUFFER,	// POSITION, NORMAL, COLOR
	LIGHTING,	// DIFFUSE LIGHT, SPECULAR LIGHT
	END,
};

enum
{
	RENDER_TARGET_SHADOW_GROUP_MEMBER_COUNT = 1,
	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 3,
	RENDER_TARGET_LIGHTING_GROUP_MEMBER_COUNT = 2,
	RENDER_TARGET_GROUP_COUNT = static_cast<uint8>(RENDER_TARGET_GROUP_TYPE::END)
};

struct RenderTarget
{
	shared_ptr<Texture> target;		// 실질적으로 그릴 대상
	float clearColor[4];			// 매 프레임마다 초기값으로 밀어주는 색상
};


// 여러가지의 render targer을 뭉쳐서 관리하는 클래스
class RenderTargetGroup
{
public:
	// 깊이 텍스처(dsTexture)는 따로 받아줌
	void Create(RENDER_TARGET_GROUP_TYPE groupType, vector<RenderTarget>& rtVec, shared_ptr<Texture> dsTexture);

	void OMSetRenderTargets(uint32 count, uint32 offset);
	void OMSetRenderTargets();

	void ClearRenderTargetView(uint32 index);
	void ClearRenderTargetView();

	shared_ptr<Texture> GetRTTexture(uint32 index) { return _rtVec[index].target; }
	shared_ptr<Texture> GetDSTexture() { return _dsTexture; }

	void WaitTargetToResource();
	void WaitResourceToTarget();

private:
	RENDER_TARGET_GROUP_TYPE		_groupType;		// 어떤 용도의 그룹으로 사용할 것이냐
	vector<RenderTarget>			_rtVec;			// RenderTargetGroup에 연결해줄 모든 rt를 벡터로 받고 있음
	uint32							_rtCount;
	shared_ptr<Texture>				_dsTexture;
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;

private:
	uint32							_rtvHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHeapBegin;
	D3D12_CPU_DESCRIPTOR_HANDLE		_dsvHeapBegin;

	// 텍스처를 렌더링하는 용도로 활용하고 있는데, 그 텍스처를 GPU에서도 그림을 그리는데도 공용으로 사용하고 있다.
	// 그래서 중간에 그려지는 과정이 같이 보여짐, 지직거리듯이 보임
	// 렌더타겟 용도로 활용하면서 동시에 일반 텍스처 용도로(리소스 용도로) 동시에 하지 않게끔 끊어줄 필요가 있음
	// 베리어를 둬서 한번 끊어준다
private:
	D3D12_RESOURCE_BARRIER			_targetToResource[8];	// 타겟에서 리소스로 넘어갈때 치는 베리어
	D3D12_RESOURCE_BARRIER			_resourceToTarget[8];	// 리소스에서 rt으로 넘어갈때 치는 베리어
};


#pragma once
#include "Texture.h"

enum class RENDER_TARGET_GROUP_TYPE : uint8
{
	SWAP_CHAIN, // BACK_BUFFER, FRONT_BUFFER
	G_BUFFER,	// POSITION, NORMAL, COLOR
	END,
};

enum
{
	RENDER_TARGET_G_BUFFER_GROUP_MEMBER_COUNT = 3,
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
};


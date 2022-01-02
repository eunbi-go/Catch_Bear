#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(uint32 size, uint32 count);

	void Clear();
	void PushData(int32 rootParamIndex, void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);

private:
	void CreateBuffer();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer;
	BYTE*					_mappedBuffer = nullptr;	// 데이터를 memcpy하기 위한 버퍼
	uint32					_elementSize = 0;			// 버퍼를 몇개 만들지 - 각 요소의 크기
	uint32					_elementCount = 0;			// 버퍼를 몇개 만들지 - 각 요소의 개수

	uint32					_currentIndex = 0;			// 버퍼를 어디까지 사용했는지

};


#pragma once

struct InstancingParams
{
	Matrix	matWorld;
	Matrix	matWV;
	Matrix	matWVP;
};

class InstancingBuffer
{
public:
	InstancingBuffer();
	~InstancingBuffer();

	void Init(uint32 maxCount = 10);

	void Clear();
	void AddData(InstancingParams& params);
	void PushData();

public:
	uint32						GetCount(); 
	ComPtr<ID3D12Resource>		GetBuffer();
	D3D12_VERTEX_BUFFER_VIEW	GetBufferView();

	void	SetID(uint64 instanceId);
	uint64	GetID();

private:
	vector<InstancingParams>	_data;


	uint64						_instanceId = 0;
	ComPtr<ID3D12Resource>		_buffer;
	D3D12_VERTEX_BUFFER_VIEW	_bufferView;

	uint32						_maxCount = 0;
	

};




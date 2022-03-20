#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Material.h"
#include "InstancingBuffer.h"
#include "StructuredBuffer.h"

Mesh::Mesh() : Object(OBJECT_TYPE::MESH)
{
}

Mesh::~Mesh()
{
}

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

void Mesh::Render(uint32 instanceCount, uint32 idx)
{
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&_vecIndexInfo[idx].bufferView);
	
	GEngine->GetGraphicsDescHeap()->CommitTable();

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(_vecIndexInfo[idx].count, instanceCount, 0, 0, 0);
}

void Mesh::Render(shared_ptr<InstancingBuffer>& buffer, uint32 idx)
{
	D3D12_VERTEX_BUFFER_VIEW bufferViews[] = { _vertexBufferView, buffer->GetBufferView() };
	GRAPHICS_CMD_LIST->IASetVertexBuffers(0, 2, bufferViews);
	GRAPHICS_CMD_LIST->IASetIndexBuffer(&_vecIndexInfo[idx].bufferView);

	GEngine->GetGraphicsDescHeap()->CommitTable();

	GRAPHICS_CMD_LIST->DrawIndexedInstanced(_vecIndexInfo[idx].count, buffer->GetCount(), 0, 0, 0);
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	_vertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	// Initialize the vertex buffer view.
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기	
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	uint32 indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = indexCount * sizeof(uint32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	ComPtr<ID3D12Resource> indexBuffer;
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	indexBuffer->Unmap(0, nullptr);

	D3D12_INDEX_BUFFER_VIEW	indexBufferView;
	indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	indexBufferView.SizeInBytes = bufferSize;

	IndexBufferInfo info =
	{
		indexBuffer,
		indexBufferView,
		DXGI_FORMAT_R32_UINT,
		indexCount
	};

	_vecIndexInfo.push_back(info);
}

void Mesh::CreateStaticMeshFromFBX(const StaticMeshInfo* meshInfo)
{
	CreateVertexBuffer(meshInfo->vertices);

	for (const vector<uint32>& buffer : meshInfo->indices)
	{
		if (buffer.empty())
		{
			return;
		}
		else
		{
			CreateIndexBuffer(buffer);
		}
	}

	return;
}

void Mesh::CreateAnimationMeshFromFBX(const StaticMeshInfo* meshInfo, vector<shared_ptr<AnimationClipInfo>> animationMat, vector<shared_ptr<CharacterBoneInfo>> boneInfo, SkinningInfo skinningInfo)
{
	CreateVertexBuffer(meshInfo->vertices);

	for (const vector<uint32>& buffer : meshInfo->indices)
	{
		if (buffer.empty())
		{
			return;
		}
		else
		{
			CreateIndexBuffer(buffer);
		}
	}

	CreateBonesAndAnimations(animationMat, boneInfo, skinningInfo);

	return;
}

void Mesh::CreateBonesAndAnimations(vector<shared_ptr<AnimationClipInfo>> animationClipInfo, vector<shared_ptr<CharacterBoneInfo>> boneInfo, SkinningInfo skinningInfo)
{
#pragma region Animation
	//vector<shared_ptr<AnimationClipInfo>> animClips = animationClipInfo;
	//
	//for (shared_ptr<AnimationClipInfo>& ac : animClips)
	//{
	//	AnimClipInfo	info = {};

	//	info.animName = ac->name;
	//	info.duration = ac->length;
	//	info.frameCount = ac->nkeyFrames;

	//	info.keyFrames.resize(ac->vecKeyFrames.size());

	//	const int32 nBones = static_cast<int32>(ac->vecKeyFrames.size());

	//	for (int32 b = 0; b < nBones; ++b)
	//	{
	//		auto& vec = ac->vecKeyFrames[b];

	//		// size: b번 뼈가 갖고 있는 프레임 행렬의 수
	//		const int32 size = static_cast<int32>(vec.size());
	//		info.keyFrames[b].resize(size);

	//		for (int32 f = 0; f < size; ++f)
	//		{
	//			AnimationFrameInfo2& kf = vec[f];
	//			KeyFrameInfo& kfInfo = info.keyFrames[b][f];

	//			kfInfo.time = kf.time;
	//			kfInfo.frame = static_cast<int32>(size);

	//			kfInfo.matTransform = kf.matOffset;
	//		}
	//	}
	//	_animClips.push_back(info);
	//}
#pragma endregion

#pragma region Bones
	vector<shared_ptr<CharacterBoneInfo>> bones = boneInfo;

	for (shared_ptr<CharacterBoneInfo>& bone : bones)
	{
		BoneInfo	bInfo = {};
		bInfo.boneName = bone->boneName;
		bInfo.matOffset = bone->toParent;
		bInfo.parentIdx = bone->parentIdx;
	}
#pragma endregion

#pragma endregion Skinning
	// Bone Offset 행렬
	SkinningInfo	skInfo = skinningInfo;
	const int32 boneCnt = static_cast<int32>(skInfo.boneOffsets.size());

	vector<Matrix>	offsetMat;
	offsetMat.resize(boneCnt);

	for (int32 i = 0; i < boneCnt; ++i)	offsetMat[i] = skInfo.boneOffsets[i];

	// OffsetMatrix StructedBuffer 세팅
	_offsetBuffer = make_shared<StructuredBuffer>();
	_offsetBuffer->Init(sizeof(Matrix), static_cast<uint32>(offsetMat.size()), offsetMat.data());

#pragma endregion
}

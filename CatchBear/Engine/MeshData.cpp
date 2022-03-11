#include "pch.h"
#include "MeshData.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"

MeshData::MeshData() : Object(OBJECT_TYPE::MESH_DATA)
{
}

MeshData::~MeshData()
{
}

shared_ptr<MeshData> MeshData::LoadMeshFromFile(const wstring& path)
{
	// FILE* fp = 

	// 1. Mesh Data
	// shared_ptr<Mesh>	mesh = 바이너리파일(메쉬)로드 함수
	// GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);

	// 2. Material Data
	// shared_ptr<Material>	material = 바이너리파일(재질)로드 함수
}

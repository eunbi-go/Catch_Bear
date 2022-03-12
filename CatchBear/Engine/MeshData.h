#pragma once
#include "Object.h"

class Mesh;
class Material;
class GameObject;

struct MeshRendererInfo
{
	shared_ptr<Mesh>				mesh;
	vector<shared_ptr<Material>>	materials;
};



class MeshData : public Object
{
public:
	MeshData();
	virtual ~MeshData();

public:
	void	LoadMeshFromFile(const wstring& path);
	
private:
	GameObject* LoadFrameHierarchyFromFile(GameObject* parent, FILE* pFile);

	void LoadMeshInfoFromFile(FILE* pFile);
	void LoadMaterialInfoFromFile(FILE* pFile);

private:
	void CreateTextures();
	void CreateMaterials();

private:
	shared_ptr<Mesh>				_mesh;
	vector<shared_ptr<Material>>	_materials;

	vector<MeshRendererInfo>		_meshRenders;

	StaticMeshInfo					_staticMeshInfo;
};


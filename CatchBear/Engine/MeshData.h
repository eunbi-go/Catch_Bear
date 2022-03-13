#pragma once
#include "Object.h"

class Mesh;
class Material;
class GameObject;

struct MeshRendererInfo
{
	shared_ptr<Mesh>		mesh;
	shared_ptr<Material>	materials;
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

protected:
	void LoadMeshInfoFromFile(FILE* pFile, bool bCharac);
	void LoadMaterialInfoFromFile(FILE* pFile);

private:
	void CreateTextures();
	void CreateMaterials();

public:
	vector<shared_ptr<GameObject>>	Instantiate();

protected:
	shared_ptr<Mesh>				_mesh;
	vector<shared_ptr<Material>>	_materials;

	vector<MeshRendererInfo>		_meshRenders;

	StaticMeshInfo					_staticMeshInfo;
};




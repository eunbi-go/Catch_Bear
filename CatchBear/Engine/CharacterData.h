#pragma once
#include "MeshData.h"

class Mesh;
class Material;
class GameObject;
class AnimationModelInfo;
class Transform;


class CharacterData : public MeshData
{
public:
	CharacterData();
	virtual ~CharacterData();

public:
	void	LoadCharacterFromFile(const wstring& path);

private:
	// 이 함수를 재귀함수로 계속 부르면서 계층구조 완성해야 함
	shared_ptr<Transform> LoadFrameHierarchyFromFile(shared_ptr<Transform> parent, FILE* pFile, bool bFirst);

private:
	void LoadSkinningInfoFromFile(FILE* pFile);
	void LoadMeshInfoFromFile(FILE* pFile);
	void LoadMaterialInfoFromFile(FILE* pFile);

	void LoadAnimationInfo(FILE* pFile);
	void CreateMaterials();
		
public:
	vector<shared_ptr<GameObject>>	Instantiate();

private:
	SkinningInfo							_skinningInfo;
	vector<wstring>							_animationFrameName;
	vector<shared_ptr<AnimationClipInfo>>	_animationClipInfo;

	shared_ptr<AnimationModelInfo>			_modelInfo;

	wstring _name;
};


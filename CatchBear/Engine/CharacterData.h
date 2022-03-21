#pragma once
//#include "Object.h"
#include "MeshData.h"

class Mesh;
class Material;
class GameObject;
class AnimationModelInfo;


class CharacterData : public MeshData
{
public:
	CharacterData();
	virtual ~CharacterData();

public:
	void	LoadCharacterFromFile(const wstring& path);

private:
	// 이 함수를 재귀함수로 계속 부르면서 계층구조 완성해야 함
	void LoadFrameHierarchyFromFile(shared_ptr<CharacterBoneInfo> parent, FILE* pFile, bool bFirst);

private:
	void LoadSkinningInfoFromFile(FILE* pFile);
	void LoadMeshInfoFromFile(FILE* pFile);
	void LoadMaterialInfoFromFile(FILE* pFile);

	void LoadAnimationInfo(FILE* pFile);

public:
	vector<shared_ptr<GameObject>>	Instantiate();

private:
	vector<shared_ptr<CharacterBoneInfo>>	_characterInfo;
	SkinningInfo							_skinningInfo;
	vector<wstring>							_animationFrameName;
	vector<shared_ptr<AnimationClipInfo>>	_animationClipInfo;

	vector<vector<AnimationFrameInfo2>>		_allFrameInfo;
	vector<shared_ptr<AnimationClipInfo2>>	_allAnimationInfos;

	shared_ptr<AnimationModelInfo>			_modelInfo;
};


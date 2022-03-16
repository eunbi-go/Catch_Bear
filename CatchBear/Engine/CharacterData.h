#pragma once
//#include "Object.h"
#include "MeshData.h"

class Mesh;
class Material;
class GameObject;

// 일단 여기에 스키닝 정보 다 넣어두고 나중에 연결시키기
struct SkinningInfo
{
	int				nSkinnings;

	vector<wstring>	boneNames;
	vector<Matrix>	boneOffsets;
	vector<XMINT4>	boneIndices;
	vector<Vec4>	boneWeights;
};

struct CharacterBoneInfo
{
	wstring		boneName;
	wstring		parentName;

	int32		nFrame;
	int32		nTexture;

	Vec3		position;
	Vec3		scale;
	Vec3		rotation;

	Matrix		toParent;
};




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

private:
	vector<shared_ptr<CharacterBoneInfo>>	_characterInfo;
	SkinningInfo							_skinningInfo;
	vector<wstring>							_animationFrameName;
	vector<shared_ptr<AnimationClipInfo>>	_animationClipInfo;
	vector<vector<AnimationFrameInfo>>		_allFrameInfo;
};


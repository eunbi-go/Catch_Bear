#include "pch.h"
#include "CharacterData.h"
#include "Mesh.h"
#include "Material.h"
#include "Resources.h"
#include "Transform.h"
#include "MeshRenderer.h"



CharacterData::CharacterData() : MeshData()
{
}

CharacterData::~CharacterData()
{
}

void CharacterData::LoadCharacterFromFile(const wstring& path)
{
	FILE* pFile = NULL;

	// wchar_t -> char*
	char* pStr;
	int iLen = WideCharToMultiByte(CP_ACP, 0, path.c_str(), -1, NULL, 0, NULL, NULL);
	pStr = new char[iLen];
	WideCharToMultiByte(CP_ACP, 0, path.c_str(), -1, pStr, iLen, 0, 0);

	fopen_s(&pFile, pStr, "rb");
	rewind(pFile);

	char pStrTocken[64] = { '\0' };

	for (; ;)
	{
		if (ReadStringFromFileForCharac(pFile, pStrTocken))
		{
			if (!strcmp(pStrTocken, "<Hierarchy>:"))
			{
				LoadFrameHierarchyFromFile(NULL, pFile, true);
			}

			else if (!strcmp(pStrTocken, "</Hierarchy>"))
			{
				CreateTextures();
				CreateMaterials();
			
				// Mesh 생성해서 Reosurces에 추가
				shared_ptr<Mesh> mesh = make_shared<Mesh>();
				mesh->CreateStaticMeshFromFBX(&_staticMeshInfo);
				mesh->SetName(path);
				GET_SINGLE(Resources)->Add<Mesh>(mesh->GetName(), mesh);


				// Material 생성해서 Reosurces에 추가
				shared_ptr<Material>	material = GET_SINGLE(Resources)->Get<Material>(_staticMeshInfo.material.name);

				MeshRendererInfo	info = {};
				info.mesh = mesh;
				info.materials = material;

				_meshRenders.push_back(info);
			}

			else if (!strcmp(pStrTocken, "<Animation>:"))
			{
				LoadAnimationInfo(pFile);

				fclose(pFile);
				return;
			}
		}
	}
}

void CharacterData::LoadFrameHierarchyFromFile(shared_ptr<CharacterBoneInfo> parent, FILE* pFile, bool bFirst)
{
	char pStrTocken[64] = { '\0' };
	UINT	nReads = 0;

	shared_ptr<CharacterBoneInfo>	cInfo = make_shared<CharacterBoneInfo>();

	if (!bFirst)
		cInfo->parentName = parent->boneName;

	for (; ;)
	{
		ReadStringFromFileForCharac(pFile, pStrTocken);

		if (!strcmp(pStrTocken, "<Frame>:"))
		{
			cInfo->nFrame = ReadIntegerFromFile(pFile);
			cInfo->nTexture = ReadIntegerFromFile(pFile);

			ReadStringFromFileForCharac(pFile, pStrTocken);
			cInfo->boneName = s2ws(pStrTocken);
		}

		else if (!strcmp(pStrTocken, "<Transform>:"))
		{
			Vec4 xmf4Rotation;

			nReads = (UINT)::fread(&cInfo->position, sizeof(float), 3, pFile);
			nReads = (UINT)::fread(&cInfo->rotation, sizeof(float), 3, pFile); //Euler Angle
			nReads = (UINT)::fread(&cInfo->scale, sizeof(float), 3, pFile);
			nReads = (UINT)::fread(&xmf4Rotation, sizeof(float), 4, pFile); //Quaternion
		}

		else if (!strcmp(pStrTocken, "<TransformMatrix>:"))
		{
			nReads = (UINT)fread(&cInfo->toParent, sizeof(float), 16, pFile);
		}

		else if (!strcmp(pStrTocken, "<Children>:"))
		{
			int		nChild = ReadIntegerFromFile(pFile);

			if (nChild > 0)
			{
				for (int i = 0; i < nChild; ++i)
					LoadFrameHierarchyFromFile(cInfo, pFile, false);
			}
		}

		else if (!strcmp(pStrTocken, "<SkinningInfo>:"))
		{
			LoadSkinningInfoFromFile(pFile);

			ReadStringFromFileForCharac(pFile, pStrTocken);
			if (!strcmp(pStrTocken, "<Mesh>:"))		LoadMeshInfoFromFile(pFile);
		}

		else if (!strcmp(pStrTocken, "<Mesh>:"))		LoadMeshInfoFromFile(pFile);

		else if (!strcmp(pStrTocken, "<Materials>:"))
		{
			LoadMaterialInfoFromFile(pFile);
		}

		else if (!strcmp(pStrTocken, "</Frame>"))
		{
			_characterInfo.push_back(cInfo);
			break;
		}
	}
}

void CharacterData::LoadSkinningInfoFromFile(FILE* pFile)
{
	char pStrTocken[64] = { '\0' };
	UINT	nReads = 0;
	int		nSkinnings = 0;

	ReadStringFromFileForCharac(pFile, pStrTocken);

	for (; ;)
	{
		ReadStringFromFileForCharac(pFile, pStrTocken);

		if (!strcmp(pStrTocken, "<BonesPerVertex>:"))
		{
			int		nVertexs = ReadIntegerFromFile(pFile);
		}

		else if (!strcmp(pStrTocken, "<Bounds>:"))
		{
			Vec3 aabbCenter, aabbExtents;

			nReads = (UINT)::fread(&aabbCenter, sizeof(Vec3), 1, pFile);
			nReads = (UINT)::fread(&aabbExtents, sizeof(Vec3), 1, pFile);
		}

		else if (!strcmp(pStrTocken, "<BoneNames>:"))
		{
			nSkinnings = ReadIntegerFromFile(pFile);
			_skinningInfo.nSkinnings = nSkinnings;

			if (nSkinnings > 0)
			{
				_skinningInfo.boneNames.resize(nSkinnings);

				for (int i = 0; i < nSkinnings; ++i)
				{
					ReadStringFromFileForCharac(pFile, pStrTocken);
					_skinningInfo.boneNames[i] = s2ws(pStrTocken);
				}
			}
		}

		else if (!strcmp(pStrTocken, "<BoneOffsets>:"))
		{
			nSkinnings = ReadIntegerFromFile(pFile);

			if (nSkinnings > 0)
			{
				_skinningInfo.boneOffsets.resize(nSkinnings);

				nReads = (UINT)fread(&_skinningInfo.boneOffsets[0], sizeof(Matrix), nSkinnings, pFile);
			}
		}

		else if (!strcmp(pStrTocken, "<BoneIndices>:"))
		{
			nSkinnings = ReadIntegerFromFile(pFile);

			if (nSkinnings > 0)
			{
				_skinningInfo.boneIndices.resize(nSkinnings);

				nReads = (UINT)fread(&_skinningInfo.boneIndices[0], sizeof(XMINT4), nSkinnings, pFile);
			}
		}

		else if (!strcmp(pStrTocken, "<BoneWeights>:"))
		{
			nSkinnings = ReadIntegerFromFile(pFile);

			if (nSkinnings > 0)
			{
				_skinningInfo.boneWeights.resize(nSkinnings);

				nReads = (UINT)fread(&_skinningInfo.boneWeights[0], sizeof(Vec4), nSkinnings, pFile);
			}
		}

		else if (!strcmp(pStrTocken, "</SkinningInfo>"))
			return;
	}
}

void CharacterData::LoadMeshInfoFromFile(FILE* pFile)
{
	char pStrTocken[64] = { '\0' };
	int	nReads, nVertices, nUVs, nNormals, nTangents, nSubMeshes;

	nVertices = ReadIntegerFromFile(pFile);

	for (; ;)
	{
		ReadStringFromFileForCharac(pFile, pStrTocken);

		if (!strcmp(pStrTocken, "<Bounds>:"))
		{
			Vec3 aabbCenter, aabbExtents;

			nReads = (UINT)::fread(&aabbCenter, sizeof(Vec3), 1, pFile);
			nReads = (UINT)::fread(&aabbExtents, sizeof(Vec3), 1, pFile);
		}

		else if (!strcmp(pStrTocken, "<ControlPoints>:"))
		{
			nReads = (UINT)fread(&nVertices, sizeof(int), 1, pFile);

			if (nVertices)
			{
				_staticMeshInfo.vertices.resize(nVertices);
				Vec3* pos = new Vec3[nVertices];

				nReads = (UINT)fread(pos, sizeof(Vec3), nVertices, pFile);

				for (int i = 0; i < nVertices; ++i)
					_staticMeshInfo.vertices[i].pos = pos[i];
			}
		}

		else if (!strcmp(pStrTocken, "<UVs>:"))
		{
			nReads = (UINT)fread(&nUVs, sizeof(int), 1, pFile);

			if (nUVs)
			{
				Vec2* uv = new Vec2[nUVs];

				nReads = (UINT)fread(uv, sizeof(Vec2), nUVs, pFile);

				for (int i = 0; i < nUVs; ++i)
					_staticMeshInfo.vertices[i].uv = uv[i];
			}
		}

		else if (!strcmp(pStrTocken, "<Normals>:"))
		{
			nReads = (UINT)fread(&nNormals, sizeof(int), 1, pFile);

			if (nNormals)
			{
				Vec3* normal = new Vec3[nNormals];

				nReads = (UINT)fread(normal, sizeof(Vec3), nNormals, pFile);

				for (int i = 0; i < nNormals; ++i)
					_staticMeshInfo.vertices[i].normal = normal[i];
			}
		}

		else if (!strcmp(pStrTocken, "<Tangents>:"))
		{
			nReads = (UINT)fread(&nTangents, sizeof(int), 1, pFile);

			if (nTangents)
			{
				Vec3* tangent = new Vec3[nTangents];

				nReads = (UINT)fread(tangent, sizeof(Vec3), nTangents, pFile);

				for (int i = 0; i < nTangents; ++i)
					_staticMeshInfo.vertices[i].tangent = tangent[i];
			}
		}

		else if (!strcmp(pStrTocken, "<SubMeshes>:"))
		{
			nSubMeshes = ReadIntegerFromFile(pFile);

			ReadStringFromFileForCharac(pFile, pStrTocken);

			if (!strcmp(pStrTocken, "<SubMesh>:"))
			{
				int nSubMesh = ReadIntegerFromFile(pFile);
				int nIndices = ReadIntegerFromFile(pFile);

				_staticMeshInfo.indices.resize(nSubMeshes);

				// 이제 인덱스 읽어옴
				UINT* subIndices = new UINT[nIndices];
				nReads = (UINT)fread(subIndices, sizeof(UINT), nIndices, pFile);

				for (int i = 0; i < nIndices; ++i)
				{
					_staticMeshInfo.indices[nSubMeshes - 1].push_back(subIndices[i]);
				}
			}
		}

		else if (!strcmp(pStrTocken, "</Mesh>"))
			return;
	}
}

void CharacterData::LoadMaterialInfoFromFile(FILE* pFile)
{
	char pStrTocken[64] = { '\0' };
	UINT	nReads = 0;
	int		nMaterials = ReadIntegerFromFile(pFile);

	// !나중에 파일에 추가해서 읽어오기!
	_staticMeshInfo.material.name = L"Player";

	ReadStringFromFileForCharac(pFile, pStrTocken);

	if (!strcmp(pStrTocken, "<Material>:"))
	{
		int		nMaterial = ReadIntegerFromFile(pFile);

		for (; ;)
		{
			ReadStringFromFileForCharac(pFile, pStrTocken);

			if (!strcmp(pStrTocken, "<AlbedoColor>:"))
				nReads = (UINT)fread(&_staticMeshInfo.material.diffuse, sizeof(float), 4, pFile);

			else if (!strcmp(pStrTocken, "<AlbedoMap>:"))
			{
				ReadStringFromFileForCharac(pFile, pStrTocken);
				_staticMeshInfo.material.diffuseTexName = s2ws(pStrTocken);
			}

			else if (!strcmp(pStrTocken, "</Materials>"))	return;
		}
	}
}

void CharacterData::LoadAnimationInfo(FILE* pFile)
{
	char	pStrTocken[64] = { '\0' };
	UINT	nReads = 0;
	int32	nAnimationSets, nFrames;

	ReadStringFromFileForCharac(pFile, pStrTocken);

	if (!strcmp(pStrTocken, "<AnimationSets>:"))	nAnimationSets = ReadIntegerFromFile(pFile);

	for (; ;)
	{
		ReadStringFromFileForCharac(pFile, pStrTocken);

		if (!strcmp(pStrTocken, "<FrameNames>:"))
		{
			int nSkins = ReadIntegerFromFile(pFile);	// 1
			int nSkin = ReadIntegerFromFile(pFile);		// 0
			ReadStringFromFileForCharac(pFile, pStrTocken);	// SkineMesh name: EvilBear
			nFrames = ReadIntegerFromFile(pFile);	// 72

			for (int i = 0; i < nFrames; ++i)
			{
				ReadStringFromFileForCharac(pFile, pStrTocken);	// FrameName

				wstring	frameName = s2ws(pStrTocken);
				_animationFrameName.push_back(frameName);
			}
		}

		else if (!strcmp(pStrTocken, "<AnimationSet>:"))
		{
			shared_ptr<AnimationClipInfo>	info = make_shared<AnimationClipInfo>();

			int nSet = ReadIntegerFromFile(pFile);

			ReadStringFromFileForCharac(pFile, pStrTocken);
			info->name = s2ws(pStrTocken);

			info->length = ReadFloatFromFile(pFile);
			info->framePerSec = ReadIntegerFromFile(pFile);
			// 해당 애니메이션의 키 프레임 개수
			info->nkeyFrames = ReadIntegerFromFile(pFile);

			info->keyFrames.resize(nFrames);

			_animationClipInfo.push_back(info);

			// i 프레임마다 애니메이션의 정보를 받아온다
			//   - 프레임 번호, 해당 프레임 재생 시간, 뼈들의 행렬
			for (int i = 0; i < info->nkeyFrames; ++i)
			{
				ReadStringFromFileForCharac(pFile, pStrTocken);

				if (!strcmp(pStrTocken, "<Transforms>:"))
				{
					AnimationFrameInfo	frameInfo;

					frameInfo.key = ReadIntegerFromFile(pFile);
					frameInfo.time = ReadFloatFromFile(pFile);
					int nSkin = ReadIntegerFromFile(pFile);

					frameInfo.matOffset.resize(nFrames);

					nReads = (UINT)fread(&frameInfo.matOffset[0], sizeof(Matrix), nFrames, pFile);
					
					_animationClipInfo[nSet]->keyFrames.push_back(frameInfo);
				}
			}
		}

		else if (!strcmp(pStrTocken, "</Animation>"))
			return;
	}
}

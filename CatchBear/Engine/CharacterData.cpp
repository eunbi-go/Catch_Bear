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
	// 객체로 생성X, 구조체 CharacterBoneInfo로 계층구조 관리

	for (; ;)
	{
		if (ReadStringFromFile(pFile, pStrTocken))
		{
			if (!strcmp(pStrTocken, "<Hierarchy>:"))
			{
				LoadFrameHierarchyFromFile(NULL, pFile);
			}
			else if (!strcmp(pStrTocken, "</Hierarchy>"))
			{
				fclose(pFile);
				return;
			}
		
			if (!strcmp(pStrTocken, "</Hierarchy>"))
			{
				fclose(pFile);
				return;
			}
		}
	}
}

void CharacterData::LoadFrameHierarchyFromFile(shared_ptr<CharacterBoneInfo> parent, FILE* pFile)
{
	char pStrTocken[64] = { '\0' };
	UINT	nReads = 0;
	int		nFrame, nTextures;

	shared_ptr<CharacterBoneInfo>	cInfo = make_shared<CharacterBoneInfo>();
	//if (!parent)
	//	cInfo->parentName = parent->boneName;

	for (; ;)
	{
		ReadStringFromFile(pFile, pStrTocken);

		if (!strcmp(pStrTocken, "<Frame>:"))
		{
			cInfo->nFrame = ReadIntegerFromFile(pFile);
			cInfo->nTexture = ReadIntegerFromFile(pFile);

			ReadStringFromFile(pFile, pStrTocken);
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
				{
					LoadFrameHierarchyFromFile(cInfo, pFile);
				}
			}
		}

		else if (!strcmp(pStrTocken, "<SkinningInfo>:"))
		{
			LoadSkinningInfoFromFile(pFile);

			ReadStringFromFile(pFile, pStrTocken);

			if (!strcmp(pStrTocken, "<Mesh>:"))
			{
				LoadMeshInfoFromFile(pFile, true);
				LoadSubMeshInfoFromFile(pFile);
			}
		}

		else if (!strcmp(pStrTocken, "<Materials>:"))
		{

		}

		else if (!strcmp(pStrTocken, "</Frame>"))	break;
	}
}

void CharacterData::LoadSkinningInfoFromFile(FILE* pFile)
{
	char pStrTocken[64] = { '\0' };
	UINT	nReads = 0;
	int		nSkinnings = 0;

	ReadStringFromFile(pFile, pStrTocken);

	for (; ;)
	{
		ReadStringFromFile(pFile, pStrTocken);

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
			skinningInfo.nSkinnings = nSkinnings;

			if (nSkinnings > 0)
			{
				skinningInfo.boneNames.resize(nSkinnings);

				for (int i = 0; i < nSkinnings; ++i)
				{
					ReadStringFromFile(pFile, pStrTocken);
					skinningInfo.boneNames[i] = s2ws(pStrTocken);
				}
			}
		}

		else if (!strcmp(pStrTocken, "<BoneOffsets>:"))
		{
			nSkinnings = ReadIntegerFromFile(pFile);

			if (nSkinnings > 0)
			{
				skinningInfo.boneOffsets.resize(nSkinnings);

				nReads = (UINT)fread(&skinningInfo.boneOffsets[0], sizeof(Matrix), nSkinnings, pFile);
			}
		}

		else if (!strcmp(pStrTocken, "<BoneIndices>:"))
		{
			nSkinnings = ReadIntegerFromFile(pFile);

			if (nSkinnings > 0)
			{
				skinningInfo.boneIndices.resize(nSkinnings);

				nReads = (UINT)fread(&skinningInfo.boneIndices[0], sizeof(XMINT4), nSkinnings, pFile);
			}
		}

		else if (!strcmp(pStrTocken, "<BoneWeights>:"))
		{
			nSkinnings = ReadIntegerFromFile(pFile);

			if (nSkinnings > 0)
			{
				skinningInfo.boneWeights.resize(nSkinnings);

				nReads = (UINT)fread(&skinningInfo.boneWeights[0], sizeof(Vec4), nSkinnings, pFile);
			}
		}

		else if (!strcmp(pStrTocken, "</SkinningInfo>"))
			return;
	}
}

void CharacterData::LoadSubMeshInfoFromFile(FILE* pFile)
{
}

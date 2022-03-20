#pragma once

#include "Component.h"
#include "Mesh.h"

class Material;
class StructuredBuffer;
class Mesh;

class AnimationController : public Component
{
public:
	AnimationController();
	virtual ~AnimationController();

public:
	void SetBones(vector<CharacterBoneInfo> bones) { _bones = bones; }
	void SetAnimClips(vector<AnimationClipInfo> anClips) { _animClips = anClips; }
	
public:
	void PushData();
	
	void Play();

public:
	virtual void FinalUpdate() override;
	
private:
	void PlayAnimation();
	Matrix	GetboneTrans(int nBone);

private:
	float							_updateTime = 0.f;
	int32							_clipIndex = 0;
	float							_fTrackPosition = 0.0f;

	vector<CharacterBoneInfo>	_bones;
	vector<AnimationClipInfo>			_animClips;

	shared_ptr<StructuredBuffer>		_boneTransform;
};


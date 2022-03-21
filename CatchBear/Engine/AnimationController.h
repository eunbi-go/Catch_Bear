#pragma once

#include "Component.h"
#include "Mesh.h"

class Material;
class StructuredBuffer;
class Mesh;
class AnimationSets;

class AnimationController : public Component
{
public:
	AnimationController();
	virtual ~AnimationController();

public:
	void SetBones(vector<shared_ptr<CharacterBoneInfo>> bones) { _bones = bones; }
	void SetAnimClips(vector<shared_ptr<AnimationClipInfo>> anClips) { _animClips = anClips; }
	
public:
	void PushData();
	
	void Play();

public:
	virtual void FinalUpdate() override;
	
public:
	void PlayAnimation();
	Matrix	GetboneTrans(int nBone);
	void UpdateBones();

private:
	float							_updateTime = 0.f;
	int32							_clipIndex = 0;
	float							_fTrackPosition = 0.0f;

	vector<shared_ptr<CharacterBoneInfo>>	_bones;
	vector<shared_ptr<AnimationClipInfo>>			_animClips;

	shared_ptr<StructuredBuffer>		_boneTransform;
};


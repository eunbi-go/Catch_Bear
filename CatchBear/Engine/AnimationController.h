#pragma once

#include "Component.h"
#include "Mesh.h"

class Material;
class StructuredBuffer;
class Mesh;
class AnimationSets;
class AnimationModelInfo;
class AnimationTrack;
class Transform;

class AnimationController : public Component
{
public:
	AnimationController();
	virtual ~AnimationController();

public:
	void SetBones(vector<shared_ptr<CharacterBoneInfo>> bones) { _bones = bones; }
	void SetAnimClips(vector<shared_ptr<AnimationClipInfo>> anClips) { _animClips = anClips; }
	void SetModelInfo(shared_ptr<AnimationModelInfo> model);

public:
	void PushData();

public:
	virtual void FinalUpdate() override;

public:
	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);
	void SetTrackEnable(int nAnimationTrack, bool bEnable);
	void SetTrackPosition(int nAnimationTrack, float fPosition);
	void SetTrackSpeed(int nAnimationTrack, float fSpeed);
	void SetTrackWeight(int nAnimationTrack, float fWeight);

	void AdvanceTime(float fElapsedTime);

private:
	float									_updateTime = 0.f;
	int32									_clipIndex = 0;
	float									_fTrackPosition = 0.0f;

	vector<shared_ptr<CharacterBoneInfo>>	_bones;
	vector<shared_ptr<AnimationClipInfo>>	_animClips;

	shared_ptr<StructuredBuffer>			_boneTransform;
	shared_ptr<AnimationModelInfo>			_modelInfo;

private:
	float								_time = 0.0f;

	int									_nAnimationTracks = 0;
	AnimationTrack*						_animTracks = NULL;

	shared_ptr<AnimationSets>			_animSets;
	vector<shared_ptr<Transform>>		_animatedTrans;
};


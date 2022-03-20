#include "pch.h"
#include "AnimationController.h"
#include "Timer.h"
#include "Resources.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "StructuredBuffer.h"

AnimationController::AnimationController() : Component(COMPONENT_TYPE::ANIMATOR)
{
	_boneTransform = make_shared<StructuredBuffer>();
}

AnimationController::~AnimationController()
{
}

void AnimationController::PushData()
{
}

void AnimationController::Play()
{
}

void AnimationController::FinalUpdate()
{
	_updateTime += DELTA_TIME;

	PlayAnimation();
}

void AnimationController::PlayAnimation()
{
	AnimationClipInfo	animClip = _animClips[_clipIndex];

	for (int i = 0; i < animClip.keyFrames.size(); ++i)
	{
		Matrix trans;
		XMStoreFloat4x4(&trans, XMMatrixIdentity());

		if (animClip.position <= 0.0f)
			animClip.position += (animClip.length * _updateTime);

		Matrix clipTrans = GetboneTrans(i);
		trans = Add(trans, Scale(clipTrans, 1.f));

		_bones[i].toParent = trans;
	}
}

Matrix AnimationController::GetboneTrans(int nBone)
{
	Matrix	trans;
	XMStoreFloat4x4(&trans, XMMatrixIdentity());

	AnimationClipInfo	animClip = _animClips[_clipIndex];

	for (int i = 0; i < animClip.nkeyFrames - 1; ++i)
	{
		if ((animClip.position >= animClip.keyFrames[i].time) && animClip.position <= animClip.keyFrames[i + 1].time)
		{
			float t = (animClip.position - animClip.keyFrames[i].time) / (animClip.keyFrames[i + 1].time - animClip.keyFrames[i].time);
			trans = Interpolate(animClip.keyFrames[i].matOffset[nBone], animClip.keyFrames[i + 1].matOffset[nBone], t);
			break;
		}
	}

	return trans;
}

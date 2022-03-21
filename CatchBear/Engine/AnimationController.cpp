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
	// BoneTrans
	int	nBones = static_cast<int>(_bones.size());
	_boneTransform->Init(sizeof(Matrix), nBones);
	_boneTransform->PushGraphicsData(SRV_REGISTER::t7);

	// OffsetTrans
	shared_ptr<Mesh>	mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
	mesh->GetBoneOffsetBuffer()->PushGraphicsData(SRV_REGISTER::t9);
}

void AnimationController::Play()
{
}

void AnimationController::FinalUpdate()
{
	_updateTime += DELTA_TIME;

	//PlayAnimation();
}

void AnimationController::PlayAnimation()
{
	shared_ptr<AnimationClipInfo>	animClip = _animClips[_clipIndex];

	for (int i = 0; i < 72/*animClip->.size()*/; ++i)
	{
		Matrix trans;
		XMStoreFloat4x4(&trans, XMMatrixIdentity());

		if (animClip->position <= 0.0f)
			animClip->position += (animClip->length * _updateTime);

		Matrix clipTrans = GetboneTrans(i);
		trans = Add(trans, Scale(clipTrans, 1.f));

		_bones[i]->toParent = trans;
	}
}

Matrix AnimationController::GetboneTrans(int nBone)
{
	Matrix	trans;
	XMStoreFloat4x4(&trans, XMMatrixIdentity());

	shared_ptr<AnimationClipInfo>	animClip = _animClips[_clipIndex];

	for (int i = 0; i < animClip->nkeyFrames - 1; ++i)
	{
		if ((animClip->position >= animClip->keyFrames[i].time) && animClip->position <= animClip->keyFrames[i + 1].time)
		{
			float t = (animClip->position - animClip->keyFrames[i].time) / (animClip->keyFrames[i + 1].time - animClip->keyFrames[i].time);
			trans = Interpolate(animClip->keyFrames[i].matOffset[nBone], animClip->keyFrames[i + 1].matOffset[nBone], t);
			break;
		}
	}

	return trans;
}

void AnimationController::UpdateBones()
{
	//m_xmf4x4World = (pxmf4x4Parent) ? Matrix4x4::Multiply(m_xmf4x4ToParent, *pxmf4x4Parent) : m_xmf4x4ToParent;

	//if (m_pSibling) m_pSibling->UpdateTransform(pxmf4x4Parent);
	//if (m_pChild) m_pChild->UpdateTransform(&m_xmf4x4World);
}

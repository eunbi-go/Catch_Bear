#include "pch.h"
#include "AnimationController.h"
#include "Timer.h"
#include "Resources.h"
#include "Material.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "StructuredBuffer.h"
#include "AnimationTrack.h"
#include "AnimationSet.h"
#include "Transform.h"

AnimationController::AnimationController() : Component(COMPONENT_TYPE::ANIMATOR)
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::SetModelInfo(shared_ptr<AnimationModelInfo> model)
{
	_nAnimationTracks = 1;
	_animTracks = new AnimationTrack[_nAnimationTracks];

	_animSets = model->_allAnimationSets;

	_animatedTrans.resize(model->_vecAnimatedFrame.size());
	
	matToParent.resize(model->_vecAnimatedFrame.size());

	for (size_t i = 0; i < model->_vecAnimatedFrame.size(); ++i)
	{
		shared_ptr<Transform>	trans = make_shared<Transform>();
		trans = model->_vecAnimatedFrame[i];
		_animatedTrans[i] = trans;

		matToParent[i] = trans->_matToParent;
	}

	_boneTransform = make_shared<StructuredBuffer>();
	_boneTransform->Init(sizeof(Matrix), static_cast<uint32>(model->_vecAnimatedFrame.size()), matToParent.data());
}

void AnimationController::PushData()
{
	// BoneTrans
	int	nBones = static_cast<int>(_animatedTrans.size());
	_boneTransform->Init(sizeof(Matrix), nBones);
	_boneTransform->PushGraphicsData(SRV_REGISTER::t7);

	//// OffsetTrans
	//shared_ptr<Mesh>	mesh = GetGameObject()->GetMeshRenderer()->GetMesh();
	//mesh->GetBoneOffsetBuffer()->PushGraphicsData(SRV_REGISTER::t9);
}

void AnimationController::AdvanceTime(float fElapsedTime)
{
	_time += fElapsedTime;

	if (_animTracks)
	{
		for (int i = 0; i < _nAnimationTracks; i++) _animTracks[i]._position += (fElapsedTime * _animTracks[i]._speed);

		for (int j = 0; j < _animatedTrans.size(); j++)
		{
			XMFLOAT4X4 xmf4x4Transform = Zero();

			for (int k = 0; k < _nAnimationTracks; k++)
			{
				if (_animTracks[k]._bEnable)
				{
					AnimationSet* pAnimationSet =
						_animSets->_animationSet[_animTracks[k]._nAnimationSet];
					pAnimationSet->SetPosition(_animTracks[k]._position);

					XMFLOAT4X4 xmf4x4TrackTransform = pAnimationSet->GetSRT(j);
					xmf4x4Transform = Add(xmf4x4Transform,
						Scale(xmf4x4TrackTransform, _animTracks[k]._weight));
				}
			}
			_animatedTrans[j]->_matToParent = xmf4x4Transform;
			matToParent[j] = xmf4x4Transform;
		}
	}
}


void AnimationController::FinalUpdate()
{
	_updateTime += DELTA_TIME;
}

void AnimationController::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet)
{
	if (_animTracks) _animTracks[nAnimationTrack]._nAnimationSet = nAnimationSet;
}

void AnimationController::SetTrackEnable(int nAnimationTrack, bool bEnable)
{
	if (_animTracks) _animTracks[nAnimationTrack].SetEnable(bEnable);
}

void AnimationController::SetTrackPosition(int nAnimationTrack, float fPosition)
{
	if (_animTracks) _animTracks[nAnimationTrack].SetPosition(fPosition);
}

void AnimationController::SetTrackSpeed(int nAnimationTrack, float fSpeed)
{
	if (_animTracks) _animTracks[nAnimationTrack].SetSpeed(fSpeed);
}

void AnimationController::SetTrackWeight(int nAnimationTrack, float fWeight)
{
	if (_animTracks) _animTracks[nAnimationTrack].SetWeight(fWeight);
}


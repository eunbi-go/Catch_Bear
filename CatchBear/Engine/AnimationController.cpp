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
#include "Engine.h"

AnimationController::AnimationController() : Component(COMPONENT_TYPE::ANIMATOR)
{
}

AnimationController::~AnimationController()
{
}

void AnimationController::SetModelInfo(shared_ptr<AnimationModelInfo> model, SkinningInfo skinInfo)
{
	_nAnimationTracks = 1;
	_animTracks = new AnimationTrack[_nAnimationTracks];

	_animSets = model->_allAnimationSets;

	// _vecAnimatedFrame: 애니메이션에 사용되는 뼈들
	_animatedTrans.resize(model->_vecAnimatedFrame.size());
	
	matToParent.resize(model->_vecAnimatedFrame.size());

	for (size_t i = 0; i < model->_vecAnimatedFrame.size(); ++i)
	{
		shared_ptr<Transform>	trans = make_shared<Transform>();
		trans = model->_vecAnimatedFrame[i];
		_animatedTrans[i] = trans;

		matToParent[i] = trans->_matWorld;
	}

	//_boneTransform = make_shared<StructuredBuffer>();
	//_boneTransform->Init(sizeof(Matrix), static_cast<uint32>(model->_vecAnimatedFrame.size()));

	// Bone Offset 행렬
	SkinningInfo	skInfo = skinInfo;
	const int32 boneCnt = static_cast<int32>(skInfo.boneOffsets.size());


	offsetMat.resize(boneCnt);

	for (int32 i = 0; i < boneCnt; ++i)	offsetMat[i] = skInfo.boneOffsets[i];

	// OffsetMatrix StructedBuffer 세팅
	//_offsetBuffer = make_shared<StructuredBuffer>();
	//_offsetBuffer->Init(sizeof(Matrix), static_cast<uint32>(offsetMat.size()), offsetMat.data());
	//_offsetBuffer->PushGraphicsData(SRV_REGISTER::t10);
}

void AnimationController::PushData()
{
	//_offsetBuffer->PushGraphicsData(SRV_REGISTER::t10);
	BoneOffsetParams	offset = {};

	for (int i = 0; i < 72; ++i)
	{
		offset.matOfset[i] = offsetMat[i];
	}
	CONST_BUFFER(CONSTANT_BUFFER_TYPE::BONE_OFFSET)->PushGraphicsData(&offset, sizeof(offset));

	AnimatedBoneParams	matrix = {};

	for (int i = 0; i < 72; ++i)
	{
		matrix.matBoneTrans[i] = matToParent[i];
	}

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::ANIMATED_BONE_TRANS)->PushGraphicsData(&matrix, sizeof(matrix));

	// BoneTrans
	// 플레이어의 계층구조에서 애니메이션에 사용되는 프레임의 월드행렬을 넘겨야 한다.
	//_boneTransform->Init(sizeof(Matrix), static_cast<uint32>(matToParent.size()), matToParent.data());
	//_boneTransform->PushGraphicsData(SRV_REGISTER::t11);

	//// OffsetTrans
	//_offsetBuffer->PushGraphicsData(SRV_REGISTER::t9);
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

			// 실제 플레이어 모델 뼈의 toParent행렬을 변경해줘야한다!
			//GetGameObject()->GetTransform()->SettoParentMat(_animatedTrans[j]->_name, _animatedTrans[j]->_matToParent);
			GetGameObject()->GetTransform()->FindTransform(_animatedTrans[j]->_name)->_matToParent = xmf4x4Transform;
		}

	}
}

void AnimationController::SetWorldMatrix()
{
	for (size_t i = 0; i < _animatedTrans.size(); ++i)
	{
		matToParent[i] = GetGameObject()->GetTransform()->FindTransform(_animatedTrans[i]->_name)->_matWorld;
	}
}


void AnimationController::FinalUpdate()
{
	//_updateTime += DELTA_TIME;
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


#include "pch.h"
#include "AnimationSet.h"

AnimationSet::AnimationSet(float length, int framePerSec, int nKeyFrames, int nSkinnings, wstring name)
{
	_length = length;
	_framePerSec = framePerSec;
	_nKeyFrames = nKeyFrames;
	_name = name;
	
	_keyFrameTimes.resize(nKeyFrames);
	_keyFrameTrans.resize(nKeyFrames);
	for (int i = 0; i < nKeyFrames; ++i)	_keyFrameTrans[i].resize(nSkinnings);
}

AnimationSet::~AnimationSet()
{
	if (!_keyFrameTrans.empty()) _keyFrameTrans.clear();
	for (int j = 0; j < _nKeyFrames; j++) if (!_keyFrameTrans[j].empty()) _keyFrameTrans.clear();
	if (!_keyFrameTimes.empty()) _keyFrameTimes.clear();
}

void AnimationSet::SetPosition(float fTrackPosition)
{
	_position = fTrackPosition;
	int i = 0;

	switch (_ntype)
	{
	case ANIMATION_TYPE_LOOP:
	{
		_position = fmod(fTrackPosition, _keyFrameTimes[_nKeyFrames - 1]);
		break;
	}

	case ANIMATION_TYPE_ONCE:
		_position = fmod(fTrackPosition, _length);
		//if (m_fPosition >= m_fLength)
		//{
		//	// 이번 동작 끝
		//	i = 1;
		//	//m_fPosition = 0.0f;
		//}

		//char buf[100];
		//sprintf_s(buf, "m_fPosition: %d\n", i);
		//OutputDebugStringA(buf);
		break;

	case ANIMATION_TYPE_PINGPONG:
		break;
	}
}

XMFLOAT4X4 AnimationSet::GetSRT(int nBone)
{
	Matrix	trans = Identity();

	for (int i = 0; i < (_nKeyFrames - 1); i++)
	{
		if ((_keyFrameTimes[i] <= _position) && (_position <= _keyFrameTimes[i + 1]))
		{
			float t = (_position - _keyFrameTimes[i]) / (_keyFrameTimes[i + 1] - _keyFrameTimes[i]);
			trans = Interpolate(_keyFrameTrans[i][nBone], _keyFrameTrans[i + 1][nBone], t);
			break;
		}
	}

	return trans;
}

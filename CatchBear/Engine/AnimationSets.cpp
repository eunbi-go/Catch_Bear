#include "pch.h"
#include "AnimationSets.h"
#include "AnimationSet.h"

AnimationSets::AnimationSets(int nAnimSets)
{
	_nAnimationSets = nAnimSets;
	_animationSet = new AnimationSet * [nAnimSets];
}

AnimationSets::~AnimationSets()
{
}

#include "pch.h"
#include "AnimationSets.h"

AnimationSets::AnimationSets(int nAnimSets)
{
	_nAnimationSets = nAnimSets;
	_animationSets.resize(nAnimSets);
}

AnimationSets::~AnimationSets()
{
}

#pragma once
class AnimationSet;

class AnimationSets
{
public:
	AnimationSets(int nAnimSets);
	~AnimationSets();

public:
	int								_nAnimationSets;
	AnimationSet**	_animationSet = NULL;
};


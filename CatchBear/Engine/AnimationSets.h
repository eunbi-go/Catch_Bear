#pragma once

class AnimationSets
{
public:
	AnimationSets(int nAnimSets);
	~AnimationSets();

public:
	int								_nAnimationSets;
	vector<vector<class AnimationSet>>	_animationSets;
};


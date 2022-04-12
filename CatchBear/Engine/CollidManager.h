#pragma once

class CollidManager
{
	DECLARE_SINGLE(CollidManager);

public:
	void Update();
	void ColiisionPlayerToStaticObj();
	void CollisionPlayerToPlayer();
};


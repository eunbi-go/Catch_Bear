#include "MonoBehaviour.h"

class GameObject;

class ScoreUI : public MonoBehaviour
{
public:
	ScoreUI();
	virtual ~ScoreUI();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;
};
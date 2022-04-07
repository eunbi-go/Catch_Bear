#pragma once
#include "MonoBehaviour.h"
class GameObject;

class ItemSlotUI : public MonoBehaviour
{
public:
	enum ITEM
	{
		SPEED_UP, TELEPORT, SHIELD, SPEED_DOWN, BLIND,
		DEBUFF_OFF, STUN, ITEM_END
	};

public:
	ItemSlotUI();
	virtual ~ItemSlotUI();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	shared_ptr<GameObject>		_itemSlot = make_shared<GameObject>();
};
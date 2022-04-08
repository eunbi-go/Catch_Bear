#pragma once
#include "MonoBehaviour.h"
#include "Item.h"

class GameObject;

class ItemSlotUI : public MonoBehaviour
{
public:
	ItemSlotUI();
	virtual ~ItemSlotUI();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	void SetItem(ITEM_EFFECT item);
	void UseItem();

	ITEM_EFFECT GetItem() { return _settingItem; }

private:
	shared_ptr<GameObject>		_itemSlot;
	ITEM_EFFECT	_settingItem = ITEM_EFFECT::END;
};
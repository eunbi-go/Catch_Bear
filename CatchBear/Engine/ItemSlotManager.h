#pragma once
#include "Item.h"
class GameObject;

class ItemSlotManager
{
	DECLARE_SINGLE(ItemSlotManager);

public:
	void SetItemSlot(int nSlot, shared_ptr<GameObject> slot);
	void AddItem(ITEM_EFFECT itemType);

private:
	shared_ptr<GameObject> _itemSlot1;
	shared_ptr<GameObject> _itemSlot2;
	shared_ptr<GameObject> _itemSlot3;
};


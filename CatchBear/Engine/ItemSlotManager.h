#pragma once
#include "Item.h"
class GameObject;

class ItemSlotManager
{
	DECLARE_SINGLE(ItemSlotManager);

public:
	void SetItemSlot(int nSlot, shared_ptr<GameObject> slot);
	void AddItem(Item::ITEM_EFFECT itemType);
	void AddItem_Cheat(Item::ITEM_EFFECT itemType);
	void UseItem(int nSlot);
	void ResetItemSlot(int nSlot);

	void UseShieldItem();

private:
	shared_ptr<GameObject> _itemSlot1;
	shared_ptr<GameObject> _itemSlot2;
	shared_ptr<GameObject> _itemSlot3;
};


#include "pch.h"
#include "ItemSlotManager.h"
#include "Engine.h"
#include "ItemSlotUI.h"
#include "GameObject.h"
#include "Player.h"

void ItemSlotManager::SetItemSlot(int nSlot, shared_ptr<GameObject> slot)
{
	switch (nSlot)
	{
	case 1:
		_itemSlot1 = slot;
		break;
	case 2:
		_itemSlot2 = slot;
		break;
	case 3:
		_itemSlot3 = slot;
		break;
	}
}

void ItemSlotManager::AddItem(ITEM_EFFECT itemType)
{
	if (static_pointer_cast<ItemSlotUI>(_itemSlot1->GetScript(0))->GetItem() == ITEM_EFFECT::END)
	{
		static_pointer_cast<ItemSlotUI>(_itemSlot1->GetScript(0))->SetItem(itemType);
	}
	else if (static_pointer_cast<ItemSlotUI>(_itemSlot2->GetScript(0))->GetItem() == ITEM_EFFECT::END)
	{
		static_pointer_cast<ItemSlotUI>(_itemSlot2->GetScript(0))->SetItem(itemType);
	}
	else if (static_pointer_cast<ItemSlotUI>(_itemSlot3->GetScript(0))->GetItem() == ITEM_EFFECT::END)
	{
		static_pointer_cast<ItemSlotUI>(_itemSlot3->GetScript(0))->SetItem(itemType);
	}
}


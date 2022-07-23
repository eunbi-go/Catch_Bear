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
	void SetItem(Item::ITEM_EFFECT item);
	void UseItem();
	void CheckItem();
	void ResetItemSlot();

	Item::ITEM_EFFECT GetItem() { return _settingItem; }
	float GetCoolTime() { return _fCoolTime; }
	const wstring& GetSettingItemName() { return _texName; }
	bool	GetIsSettingItem() { return _isSetting; }


private:
	shared_ptr<GameObject>		_itemSlot;
	Item::ITEM_EFFECT	_settingItem = Item::ITEM_EFFECT::NONE;
	float	_fCoolTime = 0.f;
	bool	_isCoolTime = false;
	bool	_isSetting = false;

public:
	wstring _texName;

};
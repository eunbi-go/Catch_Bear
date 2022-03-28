#pragma once
#include "MonoBehaviour.h"

//enum class ITEMTYPE
//{
//	COMMON, UNIQUE, TRESURE, END
//};
//
//enum class ITEM
//{
//	/*일반 아이템*/	 SPEED_UP, TELEPORT, SHIELD, SPEED_DOWN, BLIND,
//	/*유니크 아이템*/ DEBUFF_OFF, STUN,
//	/*보물	   	*/	 NONE,
//	END
//};

//struct ItemInfo
//{
//	wstring		itemName;
//	ITEM		itemType;
//
//}ITEMINFO;

// 총 플레이 타임 3분
// 일반 아이템 - 30초마다 모든 일반 아이템(5개) 하나씩 생성 -> 총 6번 생성됨
// 유니크 아이템 - 1분마다 모든 유니크 아이템(2개) 하나씩 생성 -> 총 3번 생성됨
// 보물 - 1분에 하나 생성

class Item : public MonoBehaviour
{
public:
	Item();
	//Item(ITEM itemType);
	virtual ~Item();

public:
	virtual void Update() override;

private:
	void CreateItem();

public:
	//ITEM GetItemType() { return _itemType; }


private:
	//ITEM	_itemType = ITEM::END;

	float	_curTime = 0.f;
	float	_itemTime = 5.f;

	Vec3	_itemPos = { 0.f, 0.f, 0.f };
};

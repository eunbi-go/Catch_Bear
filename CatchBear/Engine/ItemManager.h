#pragma once

// 일반 아이템 - 30초마다 모든 일반 아이템(5개) 하나씩 생성 -> 총 6번 생성됨
// 유니크 아이템 - 1분마다 모든 유니크 아이템(2개) 하나씩 생성 -> 총 3번 생성됨
// 보물 - 1분에 하나 생성
class Item;
class GameObject;

enum class ITEM_TYPE
{
	COMMON, UNIQUE, TRESURE, END
};

enum class ITEM_EFFECT
{
	/*일반 아이템*/	 SPEED_UP, TELEPORT, SHIELD, SPEED_DOWN, BLIND,
	/*유니크 아이템*/ DEBUFF_OFF, STUN,
	/*보물	   	*/	 NONE,
	END
};

class ItemManager
{
	DECLARE_SINGLE(ItemManager);

public:
	void Init();
	void Update();
	void LateUpdate();

public:
	void SetItemPosition();
	void CreateItem();

public:
	array<shared_ptr<GameObject>, 3>& GetItemArray() { return _itemArray; }
	list<shared_ptr<GameObject>>& GetCommonItemList() { return _commonItemList; }
	list<shared_ptr<GameObject>>& GetUniqueItemList() { return _uniqueItemList; }
	list<shared_ptr<GameObject>>& TreasureList() { return _treasureList; }

	ITEM_TYPE GetItemType() { return _itemType; }
	ITEM_EFFECT GetItemEffect() { return _itemEffect; }

private:
	array<shared_ptr<GameObject>, 3>	_itemArray;			// 아이템 리스트 담고있는 어레이
	list<shared_ptr<GameObject>>		_commonItemList;	
	list<shared_ptr<GameObject>>		_uniqueItemList;
	list<shared_ptr<GameObject>>		_treasureList;

	array<Vec3, 10>	_commonPosArray;	// 일반 아이템 좌표 담고있는 어레이
	array<Vec3, 10>	_uniquePosArray;	// 유니크 아이템 좌표 담고있는 어레이
	array<Vec3, 5>	_treasurePosArray;	// 보물 좌표 담고있는 어레이

	ITEM_TYPE	_itemType = ITEM_TYPE::END;
	ITEM_EFFECT	_itemEffect = ITEM_EFFECT::END;

	int		index = 0;

	// 시간은 테스트용
	float		_commonItemTimer = 0.f;
	float		_uniqueItemTimer = 0.f;
	float		_treasureTimer = 0.f;

	float		_commonItemTime = 5.f;
	float		_uniqueItemTime = 7.f;
	float		_treasureTime = 10.f;
};


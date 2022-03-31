#pragma once
#include "MonoBehaviour.h"

// 총 플레이 타임 3분
// 일반 아이템 - 30초마다 모든 일반 아이템(5개) 하나씩 생성 -> 총 6번 생성됨
// 유니크 아이템 - 1분마다 모든 유니크 아이템(2개) 하나씩 생성 -> 총 3번 생성됨
// 보물 - 1분에 하나 생성

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

class Item : public MonoBehaviour
{
public:
	Item();
	virtual ~Item();

public:
	virtual void Update() override;

public:
	ITEM_TYPE GetItemType() { return _itemType; }
	ITEM_EFFECT GetItemEffect() { return _itemEffect; }

public:
	void SetItemType(ITEM_TYPE itemType) { _itemType = itemType;  }
	void SetItemEffect(ITEM_EFFECT itemEffect) { _itemEffect = itemEffect; }
		
private:
	ITEM_TYPE		_itemType = ITEM_TYPE::END;
	ITEM_EFFECT		_itemEffect = ITEM_EFFECT::END;

	Vec3	_itemPos = { 0.f, 0.f, 0.f };
};

#pragma once
#include "MonoBehaviour.h"

enum class ITEM
{
	/*일반 아이템*/	 SPEED_UP, TELEPORT, SHIELD, SPEED_DOWN, BLIND,
	/*유니크 아이템*/ DEBUFF_OFF, STUN,
	END
};

// 총 플레이 타임 3분
// 일반 아이템은 30초에 5개 생성
// 유니크 아이템은 1분에 2개 생성 - 논의
// 병합해야돼

class Item : public MonoBehaviour
{
public:
	Item();
	Item(ITEM itemType);
	virtual ~Item();

public:
	virtual void Update() override;

public:
	ITEM GetItemType() { return _itemType; }


private:
	ITEM	_itemType = ITEM::END;

	float	_curTime = 0.f;
	float	_itemTime = 5.f;
};


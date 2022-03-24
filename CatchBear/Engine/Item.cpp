#include "pch.h"
#include "Item.h"
#include "Timer.h"

Item::Item()
{
}

Item::Item(ITEM itemType)
{
	_itemType = itemType;
}

Item::~Item()
{
}

void Item::Update()
{
	// 30초마다 일반 아이템 5개가 생성됨
	_curTime += DELTA_TIME;

	if (_curTime >= _itemTime)
	{


		_curTime = 0.f;
	}
}

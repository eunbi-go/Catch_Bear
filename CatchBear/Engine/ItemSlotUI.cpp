#include "pch.h"
#include "ItemSlotUI.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Component.h"
#include "Resources.h"
#include "Timer.h"
#include "Transform.h"

ItemSlotUI::ItemSlotUI()
{
}

ItemSlotUI::~ItemSlotUI()
{
}

void ItemSlotUI::Update()
{
	if (_isCoolTime)
	{
		if (_fCoolTime >= 0.6f)
		{
			_fCoolTime -= DELTA_TIME;

			float fScale = 5.f / _fCoolTime;
			Vec3	localScale = GetGameObject()->GetTransform()->GetLocalScale();
			GetGameObject()->GetTransform()->SetLocalScale(Vec3(localScale.x, localScale.y - fScale, localScale.z));

			float fPos = fScale / 2.f;
			Vec3	localPos = GetGameObject()->GetTransform()->GetLocalPosition();
			GetGameObject()->GetTransform()->SetLocalPosition(Vec3(localPos.x, localPos.y - fPos, localPos.z));
		}
		else
		{
			_fCoolTime = 0;
			_isCoolTime = false;

			shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"itemSlot", L"..\\Resources\\Texture\\item_slot.png");
			GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);
		}
	}
}

void ItemSlotUI::LateUpdate()
{
}

void ItemSlotUI::SetItem(ITEM_EFFECT item)
{
	_settingItem = item;
	wstring texName, texPath;

	switch (item)
	{
	case ITEM_EFFECT::SPEED_UP:
		texName = L"speed_up";
		break;
	case ITEM_EFFECT::TELEPORT:
		texName = L"teleport";
		break;
	case ITEM_EFFECT::SHIELD:
		texName = L"shield";
		break;
	case ITEM_EFFECT::SPEED_DOWN:
		texName = L"speed_down";
		break;
	case ITEM_EFFECT::BLIND:
		texName = L"blind";
		break;
	case ITEM_EFFECT::DEBUFF_OFF:
		texName = L"debuff_off";
		break;
	case ITEM_EFFECT::STUN:
		texName = L"stun";
		break;
	}

	texPath = L"..\\Resources\\Texture\\item\\" + texName + L".png";

	shared_ptr<Texture> texItem = GET_SINGLE(Resources)->Load<Texture>(texName, texPath);

	// 텍스처 바꿔줘야 함
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texItem);
}

void ItemSlotUI::UseItem()
{
	// 사용한 아이템 종류를 확인한 후, 제한시간 설정하고
	CheckItem();
	// 세팅된 아이템은 초기화
	_settingItem = ITEM_EFFECT::END;

	// after
	shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"itemSlot", L"..\\Resources\\Texture\\coolTime.png");
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);

	//// before
	//shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"itemSlot", L"..\\Resources\\Texture\\item_slot.png");
	//GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);
}

void ItemSlotUI::CheckItem()
{
	switch (_settingItem)
	{
	case ITEM_EFFECT::SPEED_UP:
		_isCoolTime = true;
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::TELEPORT:
		break;
	case ITEM_EFFECT::SHIELD:
		_isCoolTime = true;
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::SPEED_DOWN:
		_isCoolTime = true;
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::BLIND:
		_isCoolTime = true;
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::DEBUFF_OFF:
		break;
	case ITEM_EFFECT::STUN:
		_isCoolTime = true;
		_fCoolTime = 5.f;
		break;
	}
}

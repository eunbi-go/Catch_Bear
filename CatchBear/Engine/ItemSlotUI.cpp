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
	//CheckItem();
	// 세팅된 아이템은 초기화
	_settingItem = ITEM_EFFECT::END;

	//// before
	//shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"itemSlot", L"..\\Resources\\Texture\\item_slot.png");
	//GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);
}

void ItemSlotUI::CheckItem()
{
	switch (_settingItem)
	{
	case ITEM_EFFECT::SPEED_UP:
		//GetGameObject()->_isRender = false;
		break;
	case ITEM_EFFECT::TELEPORT:
		break;
	case ITEM_EFFECT::SHIELD:
		//GetGameObject()->_isRender = false;
		break;
	case ITEM_EFFECT::SPEED_DOWN:
		//GetGameObject()->_isRender = false;
		break;
	case ITEM_EFFECT::BLIND:
		//GetGameObject()->_isRender = false;
		break;
	case ITEM_EFFECT::DEBUFF_OFF:
		break;
	case ITEM_EFFECT::STUN:
		//GetGameObject()->_isRender = false;
		break;
	}
}

void ItemSlotUI::ResetItemSlot()
{
	shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"itemSlot", L"..\\Resources\\Texture\\item_slot.png");
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);
}

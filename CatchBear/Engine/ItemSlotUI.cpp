#include "pch.h"
#include "ItemSlotUI.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Component.h"
#include "Resources.h"

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

	// ÅØ½ºÃ³ ¹Ù²ãÁà¾ß ÇÔ
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texItem);
}

void ItemSlotUI::UseItem()
{
	_settingItem = ITEM_EFFECT::END;

	shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"itemSlot", L"..\\Resources\\Texture\\item_slot.png");
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);
}

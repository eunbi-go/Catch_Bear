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
	wstring texPath;
	_isSetting = true;

	switch (item)
	{
	case ITEM_EFFECT::SPEED_UP:
		_texName = L"speed_up";
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::TELEPORT:
		_texName = L"teleport";
		break;
	case ITEM_EFFECT::SHIELD:
		_texName = L"shield";
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::SPEED_DOWN:
		_texName = L"speed_down";
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::BLIND:
		_texName = L"blind";
		_fCoolTime = 5.f;
		break;
	case ITEM_EFFECT::DEBUFF_OFF:
		_texName = L"debuff_off";
		break;
	case ITEM_EFFECT::STUN:
		_texName = L"stun";
		_fCoolTime = 3.f;
		break;
	}

	texPath = L"..\\Resources\\Texture\\item\\" + _texName + L".png";

	shared_ptr<Texture> texItem = GET_SINGLE(Resources)->Load<Texture>(_texName, texPath);

	// �ؽ�ó �ٲ���� ��
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texItem);
}

void ItemSlotUI::UseItem()
{
	// ����� ������ ������ Ȯ���� ��, ���ѽð� �����ϰ�
	//CheckItem();
	// ���õ� �������� �ʱ�ȭ
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
	_fCoolTime = 0.f;
	_texName = L" ";
	_isSetting = false;
	shared_ptr<Texture> texture = GET_SINGLE(Resources)->Load<Texture>(L"itemSlot", L"..\\Resources\\Texture\\item_slot.png");
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texture);
}
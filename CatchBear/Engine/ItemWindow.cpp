#include "pch.h"
#include "ItemWindow.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Component.h"
#include "Resources.h"
#include "Transform.h"


ItemWindow::~ItemWindow()
{
}

void ItemWindow::Update()
{
}

void ItemWindow::LateUpdate()
{
}

void ItemWindow::SetItemName()
{
	//_itemName = item;
	wstring texPath = L"..\\Resources\\Texture\\item\\" + _itemName + L".png";

	shared_ptr<Texture> texItem = GET_SINGLE(Resources)->Load<Texture>(_itemName, texPath);

	// ÅØ½ºÃ³ ¹Ù²ãÁà¾ß ÇÔ
	GetGameObject()->GetMeshRenderer()->GetMaterial()->SetTexture(0, texItem);
	//printf("%s\n", ws2s(_itemName).c_str());
	GetGameObject()->_isRender = true;
}

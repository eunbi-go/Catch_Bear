#pragma once
#include "MonoBehaviour.h"
class GameObject;

class ItemWindow : public MonoBehaviour
{
public:
	ItemWindow() { }
	virtual ~ItemWindow();

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

public:
	void SetItemName();

private:
	shared_ptr<GameObject>		_itemWnd;
public:
	wstring		_itemName;
};
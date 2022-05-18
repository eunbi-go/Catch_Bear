#pragma once
#include "GameObject.h"
struct TextBlock
{
	wstring		wstrText;
	D2D1_RECT_F	d2dLayoutRect;
	IDWriteTextFormat* pdwFormat;
};

class FontDevice : public GameObject
{
public:
	FontDevice(UINT nFrame);

public:
	void Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12CommandQueue> pd3dCommandQueue);

private:
	float	_fWidth;
	float	_fHeight;

	ID3D11DeviceContext*	_pd3d11DeviceContext = NULL;
	shared_ptr<ID3D11On12Device>		_pd3d11On12Device = NULL;
	shared_ptr<IDWriteFactory>			_pd2dWriteFactory = NULL;
	shared_ptr<ID2D1Factory3>			_pd2dFactory = NULL;
	shared_ptr<ID2D1Device2>			_pd2dDevice = NULL;
	shared_ptr<ID2D1DeviceContext2>		_pd2dDeviceContext = NULL;
	shared_ptr<ID2D1SolidColorBrush>	_pd2dTextBrush = NULL;
	shared_ptr<IDWriteTextFormat>		_pdwTextFormat = NULL;

	vector<shared_ptr<ID3D11Resource>>	_vWrappedRenderTargets;
	vector<shared_ptr<ID2D1Bitmap1>>	_vd2dRenderTargets;
	vector<TextBlock>					_vTextBlocks;
};
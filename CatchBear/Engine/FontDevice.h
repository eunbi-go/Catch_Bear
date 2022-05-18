#pragma once

#include <dcommon.h>
#include <dwrite.h>
#include <d3d11on12.h>
#include <d2d1_3.h>

struct TextBlock
{
	wstring		wstrText;
	D2D1_RECT_F	d2dLayoutRect;
	IDWriteTextFormat* pdwFormat;
};

class FontDevice
{
public:
	FontDevice(UINT nFrame, ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12CommandQueue> pd3dCommandQueue);

private:
	void Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12CommandQueue> pd3dCommandQueue);

private:
	float	_fWidth;
	float	_fHeight;

	vector<shared_ptr<ID3D11Resource>>	_vWrappedRenderTargets;
	vector<shared_ptr<ID2D1Bitmap1>>	_vd2dRenderTargets;
	vector<TextBlock>					_vTextBlocks;
};
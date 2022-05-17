#pragma once

#include <dcommon.h>
#include <dwrite.h>

struct TextBlock
{
	wstring		wstrText;
	D2D1_RECT_F	d2dLayoutRect;
	IDWriteTextFormat* pdwFormat;
};

class FontDevice
{
public:
	FontDevice(UINT nFrame, shared_ptr<ID3D12Device> pd3dDevice, shared_ptr<ID3D12CommandQueue> pd3dCommandQueue);

private:
	float	_fWidth;
	float	_fHeight;

	vector<shared_ptr<ID3D11Resource>>	_vWrappedRenderTargets;
	vector<shared_ptr<ID2D1Bitmap1>>	_vd2dRenderTargets;
	vector<TextBlock>					_vTextBlocks;
};
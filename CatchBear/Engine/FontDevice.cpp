#include "pch.h"
#include "FontDevice.h"
#include "Engine.h"

FontDevice::FontDevice(UINT nFrame)
{
	_fWidth = 0.f;
	_fWidth = 0.f;
	_vWrappedRenderTargets.resize(nFrame);
	_vd2dRenderTargets.resize(nFrame);
	_vTextBlocks.resize(1);
}

void FontDevice::Initialize(ComPtr<ID3D12Device> pd3dDevice, ComPtr<ID3D12CommandQueue> pd3dCommandQueue)
{
    UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    D2D1_FACTORY_OPTIONS d2dFactoryOptions = { };

#if defined(_DEBUG) || defined(DBG)
    d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
    d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ID3D11Device* pd3d11Device = NULL;
    ID3D12CommandQueue* cmdListArr[] = { pd3dCommandQueue.Get() };
    ID3D12Device* _pd3dDevice = pd3dDevice.Get();

    ::D3D11On12CreateDevice(_pd3dDevice, d3d11DeviceFlags, nullptr, 0,
        reinterpret_cast<IUnknown**>(pd3dCommandQueue.GetAddressOf()), _countof(cmdListArr), 0,
        (ID3D11Device**)&pd3d11Device, (ID3D11DeviceContext**)&_pd3d11DeviceContext, nullptr);

    HRESULT hres = pd3d11Device->QueryInterface(__uuidof(ID3D11On12Device), (void**)&_pd3d11On12Device);
    pd3d11Device->Release();

#if defined(_DEBUG) || defined(DBG)
    ID3D12InfoQueue* pd3dInfoQueue;
    if (SUCCEEDED(pd3dDevice->QueryInterface(IID_PPV_ARGS(&pd3dInfoQueue))))
    {
        D3D12_MESSAGE_SEVERITY pd3dSeverities[] =
        {
            D3D12_MESSAGE_SEVERITY_INFO,
        };

        D3D12_MESSAGE_ID pd3dDenyIds[] =
        {
            D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE,
        };

        D3D12_INFO_QUEUE_FILTER d3dInforQueueFilter = { };
        d3dInforQueueFilter.DenyList.NumSeverities = _countof(pd3dSeverities);
        d3dInforQueueFilter.DenyList.pSeverityList = pd3dSeverities;
        d3dInforQueueFilter.DenyList.NumIDs = _countof(pd3dDenyIds);
        d3dInforQueueFilter.DenyList.pIDList = pd3dDenyIds;

        pd3dInfoQueue->PushStorageFilter(&d3dInforQueueFilter);
    }
    pd3dInfoQueue->Release();
#endif

    IDXGIDevice* pdxgiDevice = NULL;
    _pd3d11On12Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pdxgiDevice);

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, (void**)&_pd2dFactory);
    HRESULT hResult = _pd2dFactory->CreateDevice(pdxgiDevice, (ID2D1Device2**)&_pd2dDevice);
    _pd2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, (ID2D1DeviceContext2**)&_pd2dDeviceContext);

    _pd2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    _pd2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), (ID2D1SolidColorBrush**)&_pd2dTextBrush);

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&_pd2dWriteFactory);
    pdxgiDevice->Release();
}

void FontDevice::Resize(UINT nWidth, UINT nHeight)
{
    _fWidth = static_cast<float>(nWidth);
    _fHeight = static_cast<float>(nHeight);

    D2D1_BITMAP_PROPERTIES1 d2dBitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

    for (UINT i = 0; i < GetRenderTargetsCount(); ++i) {
        D3D11_RESOURCE_FLAGS    d3d11Flags = { D3D11_BIND_RENDER_TARGET };
        ComPtr<ID3D12Resource> resource = GEngine->GetRTGroup(RENDER_TARGET_GROUP_TYPE::SWAP_CHAIN)->GetRTTexture(i)->GetTex2D().Get();

        _pd3d11On12Device->CreateWrappedResource(resource.Get(), &d3d11Flags,
            D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT,
            IID_PPV_ARGS(&_vWrappedRenderTargets[i]));

        IDXGISurface* pdxgiSurface = NULL;
        _vWrappedRenderTargets[i]->QueryInterface(__uuidof(IDXGISurface), (void**)&pdxgiSurface);
        _pd2dDeviceContext->CreateBitmapFromDxgiSurface(pdxgiSurface, &d2dBitmapProperties, &_vd2dRenderTargets[i]);
        pdxgiSurface->Release();
    }

    if (_pd2dDeviceContext) _pd2dDeviceContext->Release();
    _pd2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &_pd2dDeviceContext);
    _pd2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    if (_pd2dTextBrush) _pd2dTextBrush->Release();
    _pd2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &_pd2dTextBrush);

    const float fFontSize = 50.f/*_fHeight / 100.0f*/;
    const float fSmallFontSize = _fHeight / 40.0f;

    _pd2dWriteFactory->CreateTextFormat(L"±Ã¼­Ã¼", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fFontSize, L"en-us", &_pdwTextFormat);

    _pdwTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    _pdwTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void FontDevice::Render(UINT nFrame)
{
    _vTextBlocks[0] = { L"ÇÑ±Û", D2D1::RectF(0.0f, 0.0f, 100.f, 100.f/*_fWidth, _fHeight*/), _pdwTextFormat };

    ID3D11Resource* ppResources[] = { _vWrappedRenderTargets[nFrame] };

    _pd2dDeviceContext->SetTarget(_vd2dRenderTargets[nFrame]);

    _pd3d11On12Device->AcquireWrappedResources(ppResources, _countof(ppResources));

    _pd2dDeviceContext->BeginDraw();
    for (auto textBlock : _vTextBlocks)
    {
        _pd2dDeviceContext->DrawText(textBlock.wstrText.c_str(), static_cast<UINT>(textBlock.wstrText.length()), textBlock.pdwFormat, textBlock.d2dLayoutRect, _pd2dTextBrush);
    }
    _pd2dDeviceContext->EndDraw();

    _pd3d11On12Device->ReleaseWrappedResources(ppResources, _countof(ppResources));
    _pd3d11DeviceContext->Flush();
}

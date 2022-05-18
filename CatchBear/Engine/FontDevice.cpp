#include "pch.h"
#include "FontDevice.h"

FontDevice::FontDevice(UINT nFrame)
{
	_fWidth = 0.f;
	_fWidth = 0.f;
	_vWrappedRenderTargets.resize(nFrame);
	_vd2dRenderTargets.resize(nFrame);
	_vTextBlocks.resize(1);
	//Initialize(pd3dDevice, pd3dCommandQueue);
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
    //ID3D12CommandQueue* _pd3dCommandQueue = pd3dCommandQueue.Get();
    //shared_ptr<ID3D12CommandQueue>	ppd3dcommandList[] = { make_shared<ID3D12CommandQueue>(pd3dCommandQueue.GetAddressOf()) };
    ID3D12CommandQueue* cmdListArr[] = { pd3dCommandQueue.Get() };
    //shared_ptr<ID3D12Device> _pd3dDevice = make_shared<ID3D12Device>(pd3dDevice.GetAddressOf());
    ID3D12Device* _pd3dDevice = pd3dDevice.Get();


    ::D3D11On12CreateDevice(_pd3dDevice, d3d11DeviceFlags, nullptr, 0,
        reinterpret_cast<IUnknown**>(pd3dCommandQueue.GetAddressOf()), _countof(cmdListArr), 0,
        (ID3D11Device**)&pd3d11Device, (ID3D11DeviceContext**)&_pd3d11DeviceContext, nullptr);

    //pd3d11Device->QueryInterface(__uuidof(ID3D11On12Device), (void**)&_pd3d11On12Device);
    //pd3d11Device->Release();

//#if defined(_DEBUG) || defined(DBG)
    //ID3D12InfoQueue* pd3dInfoQueue;
    //if (SUCCEEDED(pd3dDevice->QueryInterface(IID_PPV_ARGS(&pd3dInfoQueue))))
    //{
    //    D3D12_MESSAGE_SEVERITY pd3dSeverities[] =
    //    {
    //        D3D12_MESSAGE_SEVERITY_INFO,
    //    };

    //    D3D12_MESSAGE_ID pd3dDenyIds[] =
    //    {
    //        D3D12_MESSAGE_ID_INVALID_DESCRIPTOR_HANDLE,
    //    };

    //    D3D12_INFO_QUEUE_FILTER d3dInforQueueFilter = { };
    //    d3dInforQueueFilter.DenyList.NumSeverities = _countof(pd3dSeverities);
    //    d3dInforQueueFilter.DenyList.pSeverityList = pd3dSeverities;
    //    d3dInforQueueFilter.DenyList.NumIDs = _countof(pd3dDenyIds);
    //    d3dInforQueueFilter.DenyList.pIDList = pd3dDenyIds;

    //    pd3dInfoQueue->PushStorageFilter(&d3dInforQueueFilter);
    //}
    //pd3dInfoQueue->Release();
//#endif

    //IDXGIDevice* pdxgiDevice = NULL;
    //_pd3d11On12Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pdxgiDevice);

    //D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory3), &d2dFactoryOptions, (void**)&_pd2dFactory);
    //HRESULT hResult = _pd2dFactory->CreateDevice(pdxgiDevice, (ID2D1Device2**)&_pd2dDevice);
    //_pd2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, (ID2D1DeviceContext2**)&_pd2dDeviceContext);

    //_pd2dDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
    //_pd2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), (ID2D1SolidColorBrush**)&_pd2dTextBrush);

    //DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&_pd2dWriteFactory);
    //pdxgiDevice->Release();
}

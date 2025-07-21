// Copyright 2025 LINE Plus Corporation
// 
// LINE Plus Corporation licenses this file to you under the Apache License,
// version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at:
// 
//   https://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.


#include "pch.h"
#include "VideoReceiver.h"
#include <windows.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <math.h>

using namespace winrt::Windows::Graphics::Imaging;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Graphics::Display;

// SafeRelease inline function.
template <class Interface> inline void SafeRelease(
    Interface** interfaceToRelease
)
{
    if (*interfaceToRelease != nullptr)
    {
        (*interfaceToRelease)->Release();

        (*interfaceToRelease) = nullptr;
    }
}

VideoReceiver::VideoReceiver() {
    CreateDeviceIndependentResources(); 
}

VideoReceiver::~VideoReceiver() {
    if (m_videoBitmap != nullptr) {
        m_videoBitmap->Release();
    }
}

void VideoReceiver::OnVideo(const PlanetKit::SVideoFrame& sVideoFrame, PlanetKit::UserIdPtr pUserID) {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::ignore = pUserID;

    if (m_d2dDeviceContext == nullptr) {
        return;
    }

    HRESULT hr = S_OK;

    PVOID pvBuffer = sVideoFrame.pbuffer;
    uint32_t unWidth = sVideoFrame.unWidth;
    uint32_t unHeight = sVideoFrame.unHeight;

    hr = DrawToContext(pvBuffer, unWidth, unHeight, m_d2dDeviceContext.get()); // FALSE specifies drawing to screen.

    if (SUCCEEDED(hr)) {
        // Present the swap chain immediately
        hr = m_pSwapChain->Present(0, 0);
    }
}

void VideoReceiver::DrawBlack() {
    HRESULT hr = S_OK;

    //hr = CreateDeviceResources();

    if (m_d2dDeviceContext.get() == nullptr) {
        return;
    }

    // create white bitmap
    ID2D1Bitmap* pD2DBackgroundBitmap = nullptr;

    CreateBackgroundBitmap(m_d2dDeviceContext.get(), &pD2DBackgroundBitmap);

    auto d2dContext = m_d2dDeviceContext.get();

    auto size = DesktopResolution();
    D2D1_RECT_F destBackgroundRect = { 0.0f, 0.0f, (float)size.Width, (float)size.Height };
    D2D1_RECT_F sourceRect = { 0.0f, 0.0f, (float)size.Width, (float)size.Height };

    d2dContext->BeginDraw();

    // draw background
    d2dContext->DrawBitmap(
        pD2DBackgroundBitmap,
        &destBackgroundRect,
        1,
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        &sourceRect
    );

    hr = d2dContext->EndDraw();

    pD2DBackgroundBitmap->Release();

    if (SUCCEEDED(hr)) {
        // Present the swap chain immediately
        hr = m_pSwapChain->Present(0, 0);
    }
}

HRESULT VideoReceiver::CreateDeviceIndependentResources() {
    HRESULT hr = S_OK;

    if (SUCCEEDED(hr)) {
        // Create a Direct2D factory.
        D2D1_FACTORY_OPTIONS options;
        ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
        // If the project is in a debug build, enable Direct2D debugging via SDK Layers
        options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    }
    if (SUCCEEDED(hr)) {
        // Create a WIC factory.
        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_wicFactory)
        );
    }

    return hr;
}

void VideoReceiver::SetSwapChainPanel(SwapChainPanel const& panel) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (panel == nullptr) {
        return;
    }
    uint32_t creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // Create the Direct3D device.
    winrt::com_ptr<::ID3D11Device> d3dDevice;
    D3D_FEATURE_LEVEL supportedFeatureLevel;

    HRESULT hr = S_OK;
    std::wstring msg;

    D3D_DRIVER_TYPE tryDriverTypes[] = {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE,
        D3D_DRIVER_TYPE_NULL,
        D3D_DRIVER_TYPE_SOFTWARE
    };

    for (int32_t idx = 0; idx < _countof(tryDriverTypes); ++idx) {
        hr = ::D3D11CreateDevice(
            nullptr,
            (D3D_DRIVER_TYPE)idx,
            0,
            creationFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            d3dDevice.put(),
            &supportedFeatureLevel,
            nullptr);

        if (SUCCEEDED(hr)) {
            break;
        }
    }

    if (FAILED(hr)) {
        return;
    }

    // Get the Direct3D device.
    winrt::com_ptr<::IDXGIDevice> dxgiDevice{
        d3dDevice.as<::IDXGIDevice>() };

    winrt::com_ptr<::ID2D1Device> d2dDevice;
    ::D2D1CreateDevice(dxgiDevice.get(), nullptr, d2dDevice.put());

    hr = d2dDevice->CreateDeviceContext(
        D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
        m_d2dDeviceContext.put()
    );

    if (FAILED(hr)) {
        return;
    }

    // Get the DXGI adapter.
    winrt::com_ptr<::IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(dxgiAdapter.put());

    // Get the DXGI factory.
    winrt::com_ptr< ::IDXGIFactory2 > dxgiFactory;
    dxgiFactory.capture(dxgiAdapter, &IDXGIAdapter::GetParent);

    auto size = DesktopResolution();

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc{ 0 };

    // these two values mean max size.
    swapChainDesc.Width = size.Width;
    swapChainDesc.Height = size.Height;

    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // This is the most common swapchain format.
    swapChainDesc.Stereo = false;
    swapChainDesc.SampleDesc.Count = 1; // Don't use multi-sampling.
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; // We recommend using this swap effect for all applications.
    swapChainDesc.Flags = 0;

    // Create a swap chain by calling IDXGIFactory2::CreateSwapChainForComposition.
    dxgiFactory->CreateSwapChainForComposition(
        d3dDevice.get(),
        &swapChainDesc,
        nullptr,
        m_pSwapChain.put());
    auto panelNative{ panel.as<ISwapChainPanelNative>() };

    winrt::hresult result{ panelNative->SetSwapChain(m_pSwapChain.get()) };
    winrt::check_hresult(result);

    hr = CreateDeviceResources();
}

static void I420ToARGB(const uint8_t* y_plane, int y_stride,
    const uint8_t* u_plane, int u_stride,
    const uint8_t* v_plane, int v_stride,
    uint8_t* argb_buffer, int argb_stride,
    int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int y_index = y * y_stride + x;
            int uv_index = (y / 2) * u_stride + (x / 2);

            uint8_t Y = y_plane[y_index];
            uint8_t U = u_plane[uv_index];
            uint8_t V = v_plane[uv_index];

            int C = Y - 16;
            int D = U - 128;
            int E = V - 128;

            int R = (298 * C + 409 * E + 128) >> 8;
            int G = (298 * C - 100 * D - 208 * E + 128) >> 8;
            int B = (298 * C + 516 * D + 128) >> 8;

            R = R < 0 ? 0 : (R > 255 ? 255 : R);
            G = G < 0 ? 0 : (G > 255 ? 255 : G);
            B = B < 0 ? 0 : (B > 255 ? 255 : B);

            int argb_index = y * argb_stride + x * 4;
            argb_buffer[argb_index + 0] = B;
            argb_buffer[argb_index + 1] = G;
            argb_buffer[argb_index + 2] = R;
            argb_buffer[argb_index + 3] = 255; // Alpha
        }
    }
}

// Creates a Direct2D bitmap from the specified file name.
HRESULT VideoReceiver::LoadBitmapFromFrame(PVOID pvBuffer, uint32_t unWidth, uint32_t unHeight, ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap) {
    HRESULT hr;
    IWICBitmap* pBitmap = nullptr;

    std::vector<BYTE> brga_buffer(unWidth * unHeight * 4);

    I420ToARGB((const uint8_t*)pvBuffer, unWidth,
        (const uint8_t*)pvBuffer + unWidth * unHeight, unWidth / 2,
        (const uint8_t*)pvBuffer + unWidth * unHeight * 5 / 4, unWidth / 2,
        brga_buffer.data(), unWidth * 4,
        unWidth, unHeight);

    uint32_t unStride = unWidth * 4;
    uint32_t unSize = unWidth * unHeight * 4;

    hr = m_wicFactory->CreateBitmapFromMemory(unWidth, unHeight, GUID_WICPixelFormat32bppPBGRA, unStride, unSize, brga_buffer.data(), &pBitmap);

    if (SUCCEEDED(hr)) {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = d2dContext->CreateBitmapFromWicBitmap(
            pBitmap,
            nullptr,
            bitmap
        );

        pBitmap->Release();
    }
    return hr;
}

HRESULT VideoReceiver::CreateBackgroundBitmap(ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap) {
    HRESULT hr;
    IWICBitmap* pBitmap = nullptr;

    constexpr int nPixel = 1;

    std::vector<BYTE> brga_buffer(nPixel * nPixel * 4);

    for (int y = 0; y < nPixel; y++) {
        for (int x = 0; x < nPixel; x++) {
            // BGRA order
            int bgra_offset = (y * nPixel + x) * 4;
            brga_buffer[bgra_offset] = static_cast<unsigned char>(0);
            brga_buffer[bgra_offset + 1] = static_cast<unsigned char>(0);
            brga_buffer[bgra_offset + 2] = static_cast<unsigned char>(0);
            brga_buffer[bgra_offset + 3] = 255; // Alpha channel, set to 255 (fully opaque)
        }
    }

    uint32_t unStride = nPixel * 4;
    uint32_t unSize = nPixel * nPixel * 4;

    hr = m_wicFactory->CreateBitmapFromMemory(nPixel, nPixel, GUID_WICPixelFormat32bppPBGRA, unStride, unSize, brga_buffer.data(), &pBitmap);

    if (SUCCEEDED(hr)) {
        try {
            // Create a Direct2D bitmap from the WIC bitmap.
            hr = d2dContext->CreateBitmapFromWicBitmap(
                pBitmap,
                nullptr,
                bitmap
            );
        }
        catch (const std::exception& e) {
            std::ignore = e;
        };


        pBitmap->Release();
    }
    return hr;
}

HRESULT VideoReceiver::CreateDeviceResources() {
    HRESULT hr = S_OK;

    if (m_pSwapChain == nullptr) {
        return S_FALSE;
    }

    if (m_videoBitmap != nullptr) {
        m_videoBitmap->Release();
        m_videoBitmap = nullptr;
    }

    IDXGISurface* surface = nullptr;
    if (SUCCEEDED(hr)) {
        // Get a surface from the swap chain.
        hr = m_pSwapChain->GetBuffer(
            0,
            IID_PPV_ARGS(&surface)
        );
    }

    if (SUCCEEDED(hr)) {
        // Create a bitmap pointing to the surface.
        D2D1_BITMAP_PROPERTIES1 properties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(
                DXGI_FORMAT_B8G8R8A8_UNORM,
                //DXGI_FORMAT_420_OPAQUE,
                D2D1_ALPHA_MODE_IGNORE
            )
        );

        hr = m_d2dDeviceContext->CreateBitmapFromDxgiSurface(
            surface,
            &properties,
            &m_videoBitmap
        );
    }
    if (SUCCEEDED(hr)) {
        // Set the bitmap as the target of our device context.
        m_d2dDeviceContext->SetTarget(m_videoBitmap);
    }

    SafeRelease(&surface);


    return hr;
}

// Draws the scene to a rendering device context or a printing device context.
// If the "printing" parameter is set, this function will add margins to
// the target and render the text across two pages. Otherwise, it fits the
// content to the target and renders the text in one block.
HRESULT VideoReceiver::DrawToContext(PVOID pvBuffer, uint32_t unWidth, uint32_t unHeight, ID2D1DeviceContext* d2dContext) {
    HRESULT hr = S_OK;

    ID2D1Bitmap* pD2DBitmap = nullptr;

    hr = LoadBitmapFromFrame(pvBuffer, unWidth, unHeight, d2dContext, &pD2DBitmap);
    if (FAILED(hr)) {
        return hr;
    }

    auto deskTopSize = DesktopResolution();
    D2D1_RECT_F destRect = { 0.0f, 0.0f, (float)deskTopSize.Width, (float)deskTopSize.Height };
    D2D1_RECT_F sourceRect = { 0.0f, 0.0f, (float)unWidth, (float)unHeight };

    d2dContext->BeginDraw();

    try {
        d2dContext->DrawBitmap(
            pD2DBitmap,
            &destRect,
            1,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            &sourceRect
        );
    }
    catch (const std::exception& e) {
        std::ignore = e;
    };

    hr = d2dContext->EndDraw();

    pD2DBitmap->Release();

    return hr;
}

winrt::Windows::Graphics::SizeInt32 VideoReceiver::DesktopResolution() {
    RECT rtDesktop;

    const HWND hDesktop = ::GetDesktopWindow();
    GetWindowRect(hDesktop, &rtDesktop);

    return winrt::Windows::Graphics::SizeInt32{ rtDesktop.right, rtDesktop.bottom };

}
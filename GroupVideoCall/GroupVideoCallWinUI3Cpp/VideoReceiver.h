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

#pragma once

#include <d2d1_1.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Windows.h>
#include <WinUser.h>

#include <mutex>

#include <xpsobjectmodel_1.h>
#include <DocumentTarget.h>
#include <microsoft.ui.xaml.media.dxinterop.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/Windows.Graphics.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml::Controls;

#include "PlanetKitCameraController.h"

class VideoReceiver final : public PlanetKit::IVideoReceiver {
public:
    VideoReceiver();
    virtual ~VideoReceiver();

    void OnVideo(const PlanetKit::SVideoFrame& sVideoFrame, PlanetKit::UserIdPtr pUserID) final;
    void SetSwapChainPanel(SwapChainPanel const& panel);
    void DrawBlack();
private:
    HRESULT CreateDeviceIndependentResources();
    HRESULT LoadBitmapFromFrame(PVOID pvBuffer, uint32_t unWidth, uint32_t unHeight, ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap);
    HRESULT CreateBackgroundBitmap(ID2D1DeviceContext* d2dContext, ID2D1Bitmap** bitmap);
    HRESULT CreateDeviceResources();
    HRESULT DrawToContext(PVOID pvBuffer, uint32_t unWidth, uint32_t unHeight, ID2D1DeviceContext* d2dContext);

    winrt::Windows::Graphics::SizeInt32 DesktopResolution();

private:
    std::mutex m_mutex;

    com_ptr<IDXGISwapChain1> m_pSwapChain;
    com_ptr<::ID2D1DeviceContext> m_d2dDeviceContext;
    IWICImagingFactory2* m_wicFactory = nullptr;
    ID2D1Bitmap1* m_videoBitmap = nullptr;
};
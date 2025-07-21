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
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "GroupVideoCallJoinPage.xaml.h"

#include <winrt/Microsoft.UI.Windowing.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;


// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    int32_t MainWindow::MyProperty() {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */) {
        throw hresult_not_implemented();
    }

    void MainWindow::InitializeComponent() {
        MainWindowT::InitializeComponent();

        Resize(AppWindow(), 800, 700);
        CenterWindow(AppWindow());

        // Creates a ViewModel for GroupAudioCall.
        m_groupVideoCallViewModel = winrt::make<GroupVideoCallViewModel>(DispatcherQueue());

        m_participantViewModel = winrt::make<ParticipantViewModel>(DispatcherQueue());

        m_mainWindowViewModel = winrt::make<MainWindowViewModel>(m_groupVideoCallViewModel, m_participantViewModel);

        ToMainPage();
    }

    void MainWindow::ToMainPage() {
        Windows::UI::Xaml::Interop::TypeName pageTypeName = winrt::xaml_typename<winrt::GroupVideoCallWinUI3Cpp::GroupVideoCallJoinPage>();
        Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionInfo slide;
        slide.Effect(Microsoft::UI::Xaml::Media::Animation::SlideNavigationTransitionEffect::FromRight);

        Windows::Foundation::Collections::IMap<hstring, Windows::Foundation::IInspectable> groupVideoCallJoinData = winrt::single_threaded_map<hstring, Windows::Foundation::IInspectable>();

        groupVideoCallJoinData.Insert(L"mainFrame", mainFrame());
        groupVideoCallJoinData.Insert(L"groupVideoCallViewModel", m_groupVideoCallViewModel);
        groupVideoCallJoinData.Insert(L"participantViewModel", m_participantViewModel);

        mainFrame().Navigate(pageTypeName, box_value(groupVideoCallJoinData), slide);
    }

    void MainWindow::Resize(winrt::Microsoft::UI::Windowing::AppWindow& appWindow, int32_t nWidth, int32_t nHeight) {
        auto presenter = appWindow.Presenter();
        auto overlappedPresenter = presenter.as<winrt::Microsoft::UI::Windowing::OverlappedPresenter>();

        winrt::Windows::Graphics::SizeInt32 scaleForm = {
            nWidth,
            nHeight
        };

        appWindow.ResizeClient(scaleForm);

        overlappedPresenter.IsMaximizable(false);
        overlappedPresenter.IsMinimizable(true);
        overlappedPresenter.IsResizable(false);
    }

    GroupVideoCallWinUI3Cpp::ParticipantViewModel MainWindow::participantViewModel() {
        return m_participantViewModel;
    }

    void MainWindow::CenterWindow(winrt::Microsoft::UI::Windowing::AppWindow& appWindow) {
        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
        int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

        auto size = appWindow.Size();

        int nLeft = nScreenWidth / 2 - size.Width / 2;
        int nTop = nScreenHeight / 2 - size.Height / 2;
        appWindow.Move(winrt::Windows::Graphics::PointInt32{ nLeft, nTop });
    }
}

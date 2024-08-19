// Copyright 2024 LINE Plus Corporation
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

#include "GroupAudioCallViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::GroupAudioCallWinUI3Cpp::implementation {
    int32_t MainWindow::MyProperty() {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */) {
        throw hresult_not_implemented();
    }

    void MainWindow::InitializeComponent() {
        MainWindowT::InitializeComponent();

        // Creates a ViewModel for GroupAudioCall.
        m_groupAudioCallViewModel = winrt::make<GroupAudioCallViewModel>();
        // Attaches a `InfoBar` UI that is log message.
        m_groupAudioCallViewModel.LogMessageInfoBar(logMessage());
    }

    GroupAudioCallWinUI3Cpp::GroupAudioCallViewModel MainWindow::CallViewModel() {
        return m_groupAudioCallViewModel;
    }
}

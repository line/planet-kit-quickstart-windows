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

#include "MainWindow.g.h"
#include "GroupVideoCallViewModel.h"
#include "ParticipantViewModel.h"
#include "MainWindowViewModel.h"

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {

    struct MainWindow : MainWindowT<MainWindow> {
        MainWindow() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void InitializeComponent();
        void Resize(winrt::Microsoft::UI::Windowing::AppWindow& appWindow, int32_t nWidth, int32_t nHeight);

        GroupVideoCallWinUI3Cpp::ParticipantViewModel participantViewModel();

    private:
        void CenterWindow(winrt::Microsoft::UI::Windowing::AppWindow& appWindow);
        void ToMainPage();

        GroupVideoCallWinUI3Cpp::MainWindowViewModel m_mainWindowViewModel{ nullptr };
        GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel m_groupVideoCallViewModel{ nullptr };
        GroupVideoCallWinUI3Cpp::ParticipantViewModel m_participantViewModel{ nullptr };
    };
}

namespace winrt::GroupVideoCallWinUI3Cpp::factory_implementation {
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow> {
    };
}

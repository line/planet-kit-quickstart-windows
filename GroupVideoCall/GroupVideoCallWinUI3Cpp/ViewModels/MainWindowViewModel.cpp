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
#include "MainWindowViewModel.h"
#if __has_include("MainWindowViewModel.g.cpp")
#include "MainWindowViewModel.g.cpp"
#endif

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    MainWindowViewModel::MainWindowViewModel(winrt::GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel const& groupVideoCallViewModel, winrt::GroupVideoCallWinUI3Cpp::ParticipantViewModel const& participantViewModel) {
        GroupVideoCall::GroupVideoCallController::GetInstance()->InitializePlanetKit();
        m_pEventListener = std::make_shared<EventListener>(groupVideoCallViewModel, participantViewModel);
        GroupVideoCall::GroupVideoCallController::GetInstance()->SetEventListener(m_pEventListener);
    }

    int32_t MainWindowViewModel::MyProperty() {
        throw hresult_not_implemented();
    }

    void MainWindowViewModel::MyProperty(int32_t /*value*/) {
        throw hresult_not_implemented();
    }
}

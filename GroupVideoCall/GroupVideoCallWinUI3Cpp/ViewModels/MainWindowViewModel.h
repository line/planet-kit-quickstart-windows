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

#include "MainWindowViewModel.g.h"
#include "EventListener.h"
#include "ParticipantViewModel.h"
#include "GroupVideoCallViewModel.h"

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel> {
        MainWindowViewModel(winrt::GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel const& groupVideoCallViewModel, winrt::GroupVideoCallWinUI3Cpp::ParticipantViewModel const& participantViewModel);
        int32_t MyProperty();
        void MyProperty(int32_t value);

        GroupVideoCallWinUI3Cpp::ParticipantViewModel participantViewModel();
    private:
        std::shared_ptr<EventListener> m_pEventListener;
    };
}

namespace winrt::GroupVideoCallWinUI3Cpp::factory_implementation {
    struct MainWindowViewModel : MainWindowViewModelT<MainWindowViewModel, implementation::MainWindowViewModel> {
    };
}

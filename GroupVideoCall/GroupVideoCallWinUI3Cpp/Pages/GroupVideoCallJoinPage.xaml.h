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

#include "GroupVideoCallJoinPage.g.h"

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    struct GroupVideoCallJoinPage : GroupVideoCallJoinPageT<GroupVideoCallJoinPage> {
        GroupVideoCallJoinPage() = default;
        virtual ~GroupVideoCallJoinPage();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void InitializeComponent();
        void OnNavigatedTo(Microsoft::UI::Xaml::Navigation::NavigationEventArgs const& args);

        GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel groupVideoCallViewModel();

        void OnClickJoin(::winrt::Windows::Foundation::IInspectable const& sender, ::winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        void groupVideoCallViewModelProperyChanged(IInspectable const& sender, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs const& e);

        event_token m_callViewModelEventToken;

        Microsoft::UI::Xaml::Controls::Frame m_mainFrame{ nullptr };

        GroupVideoCallWinUI3Cpp::GroupVideoCallViewModel m_groupVideoCallViewModel{ nullptr };
        GroupVideoCallWinUI3Cpp::ParticipantViewModel m_participantViewModel{ nullptr };
    };
}

namespace winrt::GroupVideoCallWinUI3Cpp::factory_implementation {
    struct GroupVideoCallJoinPage : GroupVideoCallJoinPageT<GroupVideoCallJoinPage, implementation::GroupVideoCallJoinPage> {
    };
}

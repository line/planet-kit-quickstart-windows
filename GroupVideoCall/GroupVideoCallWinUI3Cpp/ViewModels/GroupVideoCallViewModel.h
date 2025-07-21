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

#include "GroupVideoCallViewModel.g.h"

#include <memory>
#include <functional>

#include "../PlanetKit/src/GroupVideoCallController.h"

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {
    struct GroupVideoCallViewModel : GroupVideoCallViewModelT<GroupVideoCallViewModel> {
        GroupVideoCallViewModel(winrt::Microsoft::UI::Dispatching::DispatcherQueue const& queue);
        event_token PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(event_token const& token) noexcept;


        ConnectedState GroupCallConnectedState();
        void GroupCallConnectedState(ConnectedState connectedState);

        void DisconnectReason(hstring strDisconnectReason);
        hstring DisconnectReason();

        hstring UserID();

        hstring RoomID();
        void RoomID(const hstring& strRoomID);

        Microsoft::UI::Xaml::Controls::InfoBar LogMessageInfoBar();
        void LogMessageInfoBar(Microsoft::UI::Xaml::Controls::InfoBar logInfoBar);

        hstring LogTitle();
        void LogTitle(const hstring& strTitle);

        hstring LogMessage();
        void LogMessage(const hstring& strMessage);

        GroupVideoCallWinUI3Cpp::ParticipantViewModel participantViewModel();

        void participantViewModel(GroupVideoCallWinUI3Cpp::ParticipantViewModel participantViewModel);

        void JoinCoference();
        void FailedToJoin(int nFailReason);

        void OpenLogMessage(const hstring& strTitle, const hstring& strMessage);

    private:

        event<Microsoft::UI::Xaml::Data::PropertyChangedEventHandler> m_PropertyChanged;

        ConnectedState m_connectedState = ConnectedState::disconnected;
        hstring m_strDisconnectReason;

        hstring m_strUserID;
        hstring m_strRoomID;

        Microsoft::UI::Xaml::Controls::InfoBar m_logInfoBar{ nullptr };
        hstring m_strLogTitle;
        hstring m_strLogMessage;

        GroupVideoCallWinUI3Cpp::ParticipantViewModel m_participantViewModel{ nullptr };
        winrt::Microsoft::UI::Dispatching::DispatcherQueue m_queue;
    };
}

namespace winrt::GroupVideoCallWinUI3Cpp::factory_implementation {
    struct GroupVideoCallViewModel : GroupVideoCallViewModelT<GroupVideoCallViewModel, implementation::GroupVideoCallViewModel>  {
    };
}

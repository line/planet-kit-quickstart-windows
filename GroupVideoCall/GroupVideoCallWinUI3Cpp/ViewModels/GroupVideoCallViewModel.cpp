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
#include "GroupVideoCallViewModel.h"
#if __has_include("GroupVideoCallViewModel.g.cpp")
#include "GroupVideoCallViewModel.g.cpp"
#endif

#include "../PlanetKit/src/Constants.h"

#include "EventListener.h"
#include "ParticipantViewModel.h"

namespace winrt::GroupVideoCallWinUI3Cpp::implementation {

    GroupVideoCallViewModel::GroupVideoCallViewModel(winrt::Microsoft::UI::Dispatching::DispatcherQueue const& queue) : m_queue(queue) {
        m_strUserID = GroupVideoCall::USER_ID;
    }

    event_token GroupVideoCallViewModel::PropertyChanged(Microsoft::UI::Xaml::Data::PropertyChangedEventHandler const& handler) {
        return m_PropertyChanged.add(handler);
    }

    void GroupVideoCallViewModel::PropertyChanged(event_token const& token) noexcept {
        m_PropertyChanged.remove(token);
    }

    void GroupVideoCallViewModel::FailedToJoin(int nFailReason) {
        winrt::hstring strLogMessage;
        strLogMessage = L"Fail reason(" + std::to_wstring(nFailReason) + L")";

        OpenLogMessage(L"Failed to Join", strLogMessage);

        strLogMessage = L"Failed to Join. " + strLogMessage + L"\n";
        OutputDebugStringW(strLogMessage.c_str());
    }

    ConnectedState GroupVideoCallViewModel::GroupCallConnectedState() {
        return m_connectedState;
    }

    void GroupVideoCallViewModel::GroupCallConnectedState(ConnectedState connectedState) {
        m_queue.TryEnqueue([connectedState, this]() {
            m_connectedState = connectedState;

            if (m_connectedState == ConnectedState::connected) {

                OpenLogMessage(L"Join", L"Connected");
                OutputDebugStringW(L"Connected\n");
            }
            else if(m_connectedState == ConnectedState::disconnected) {
                m_strDisconnectReason = L"Disconnected (" + m_strDisconnectReason + L")";

                OpenLogMessage(L"Leave", m_strDisconnectReason);

                m_strDisconnectReason = m_strDisconnectReason + L"\n";
                OutputDebugStringW(m_strDisconnectReason.c_str());

                GroupVideoCall::GroupVideoCallPeerList::GetInstance()->Clear();
            }

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"GroupCallConnectedState"));
        });
    }

    void GroupVideoCallViewModel::DisconnectReason(hstring strDisconnectReason) {
        m_queue.TryEnqueue([strDisconnectReason, this]() {
            m_strDisconnectReason = strDisconnectReason;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"DisconnectReason"));
        });
    }

    hstring GroupVideoCallViewModel::DisconnectReason() {
        return m_strDisconnectReason;
    }

    hstring GroupVideoCallViewModel::UserID() {
        return m_strUserID;
    }

    hstring GroupVideoCallViewModel::RoomID() {
        return m_strRoomID;
    }

    void GroupVideoCallViewModel::RoomID(const hstring& strRoomID) {
        if (m_strRoomID != strRoomID) {
            m_strRoomID = strRoomID;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"RoomID"));
        }
    }

    Microsoft::UI::Xaml::Controls::InfoBar GroupVideoCallViewModel::LogMessageInfoBar() {
        return m_logInfoBar;
    }

    void GroupVideoCallViewModel::LogMessageInfoBar(Microsoft::UI::Xaml::Controls::InfoBar logInfoBar) {
        if (m_logInfoBar != logInfoBar) {
            m_logInfoBar = logInfoBar;
        }
    }

    hstring GroupVideoCallViewModel::LogTitle() {
        return m_strLogTitle;
    }

    void GroupVideoCallViewModel::LogTitle(const hstring& strTitle) {
        if (m_strLogTitle != strTitle) {
            m_strLogTitle = strTitle;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"LogTitle"));
        }
    }

    hstring GroupVideoCallViewModel::LogMessage() {
        return m_strLogMessage;
    }

    void GroupVideoCallViewModel::LogMessage(const hstring& strMessage) {
        if (m_strLogMessage != strMessage) {
            m_strLogMessage = strMessage;

            m_PropertyChanged(*this, Microsoft::UI::Xaml::Data::PropertyChangedEventArgs(L"LogMessage"));
        }
    }

    void GroupVideoCallViewModel::OpenLogMessage(const hstring& strTitle, const hstring& strMessage) {
        LogTitle(strTitle);
        LogMessage(strMessage);
        m_logInfoBar.IsOpen(true);
    }

    GroupVideoCallWinUI3Cpp::ParticipantViewModel GroupVideoCallViewModel::participantViewModel() {
        return m_participantViewModel;
    }

    void GroupVideoCallViewModel::participantViewModel(GroupVideoCallWinUI3Cpp::ParticipantViewModel participantViewModel) {
        m_participantViewModel = participantViewModel;
    }

    void GroupVideoCallViewModel::JoinCoference() {
        GroupVideoCall::GroupVideoCallController::GetInstance()->JoinConference(m_strRoomID.c_str());
    }
}
